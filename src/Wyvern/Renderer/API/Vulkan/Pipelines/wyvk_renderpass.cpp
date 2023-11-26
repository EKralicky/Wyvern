#include "wyvk_renderpass.h"

namespace Wyvern {

WYVKRenderPass::WYVKRenderPass(WYVKSwapchain& swapchain, WYVKDevice& device)
	: m_device(device),
	m_swapchain(swapchain)
{
	createImageResources();
}

WYVKRenderPass::~WYVKRenderPass()
{
	destroyImageResources();
	destroyFramebuffers();
	// Destroy render pass
	vkDestroyRenderPass(m_device.getLogicalDevice(), m_renderPass, nullptr);
}

void WYVKRenderPass::createRenderPass()
{
	/*
	* =============================================
	*  COLOR ATTACHMENT
	* =============================================
	*/
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapchain.getImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // multisampling for msaa
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Undefined initial state. We don't care what layout the image was previously in
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // We are presenting directly to the swapchain

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0; // Index of attachment in the pAttachments list in the framebuffer 
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Specifies the attachment type this ref is reffering to

	/*
	* =============================================
	*  DEPTH ATTACHMENT (this will fail currently if useDepthAttachment is false!)
	* =============================================
	*/
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = m_depthImage->getFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1; // Index of attachment in the pAttachments list in the framebuffer 
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Specifies the attachment type this ref is reffering to

	/*
	* =============================================
	* SUBPASS
	* =============================================
	*/
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef; // This is the REFERENCE not the actual attachment. This is to have the ability to reuse attachments through a reference in different subpasses
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	/*
	* =============================================
	* SUBPASS DEPENDENCIES
	* =============================================
	*/
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Source/before subpass. EXTERNAL here represents the implicit starting subpass
	dependency.dstSubpass = 0; // Destination subpass index
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // Wait on color attachment image and depth early fragment tests before proceeding with the next subpass
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // What we are doing next basically. The operations that are waiting on the VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	// =============================================
	// NOTE
	// =============================================
	// The layout(location = 0) out vec4 outColor directive in the GLSL fragment shader code is an output variable declaration.
	// It means that outColor is an output from the fragment shader and its output location is 0. 
	// This location directly corresponds to the index of the attachment in the subpass description.
	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CALL(vkCreateRenderPass(m_device.getLogicalDevice(), &renderPassInfo, nullptr, &m_renderPass), "Failed to create Renderpass!");
}

void WYVKRenderPass::createFramebuffers()
{
	std::vector<VkImageView>& imageViews = m_swapchain.getImageViews();
	m_framebuffers.resize(imageViews.size());

	for (size_t i = 0; i < imageViews.size(); i++) {

		std::array<VkImageView, 2> attachments = {
			imageViews[i],
			m_depthImage->getImageView()
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_swapchain.getExtent().width;
		framebufferInfo.height = m_swapchain.getExtent().height;
		framebufferInfo.layers = 1;

		VK_CALL(vkCreateFramebuffer(m_device.getLogicalDevice(), &framebufferInfo, nullptr, &m_framebuffers[i]), "Failed to create framebuffer!");
	}
}

void WYVKRenderPass::createImageResources()
{
	m_depthImage = std::make_unique<WYVKImage>(m_device, WYVKImage::ImageType::WYVK_DEPTH_IMAGE, m_swapchain.getExtent().width, m_swapchain.getExtent().height,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	m_depthImage->createImageView(VK_IMAGE_ASPECT_DEPTH_BIT);
}

void WYVKRenderPass::destroyImageResources()
{
	m_depthImage.reset();
}

void WYVKRenderPass::destroyFramebuffers()
{
	// Destroy all framebuffers
	for (auto framebuffer : m_framebuffers) {
		vkDestroyFramebuffer(m_device.getLogicalDevice(), framebuffer, nullptr);
	}
}

}
//VkAttachmentDescription WYVKRenderPass::createAttachment()
//{
//	// Will come back once i have a need for multiple subpasses
//}
//
//*
//* A subpass is a rendering operation that performs on an attachement (image). They are called attachments be cause the images are attached to the framebuffer.
//* Attachments that get rendered to are called "Render Targets"
//*/
//void WYVKRenderPass::addSubpass() // TODO: decouple attachment type, attachment ref from this function and pass them in as variables
//{
//	// Will come back once i have a need for multiple subpasses
//}
//
//}



