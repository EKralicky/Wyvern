#include "wyvk_renderpass.h"

namespace Wyvern {

WYVKRenderPass::WYVKRenderPass(WYVKSwapchain& swapchain, WYVKDevice& device)
	: m_device(device),
	m_swapchain(swapchain)
{
}

void WYVKRenderPass::destroy()
{
	vkDestroyRenderPass(m_device.getLogicalDevice(), m_renderPass, nullptr);
}

void WYVKRenderPass::createRenderPass()
{
	/*
	* =============================================
	* ATTACHMENT
	* =============================================
	*/
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapchain.getImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // multisampling for msaa

	// VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
	// VK_ATTACHMENT_LOAD_OP_CLEAR : Clear the values to a constant at the start
	// VK_ATTACHMENT_LOAD_OP_DONT_CARE : Existing contents are undefined; we don't care about them
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

	// VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
	// VK_ATTACHMENT_STORE_OP_DONT_CARE : Contents of the framebuffer will be undefined after the rendering operation
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// We don't care about depth stenciling for now
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	// Images need to be formatted before they go to the next stage for rendering.
	// 
	// Options for finalLayout:
	// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
	// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : Images to be presented in the swap chain
	// VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : Images to be used as destination for a memory copy operation. most likely for post processing?

	//specifies which layout the image will have before the render pass begins.
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Undefined initial state. We don't care what layout the image was previously in

	//specifies the layout to automatically transition to when the render pass finishes.
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // We are presenting directly to the swapchain

	/*
	* =============================================
	* ATTACHMENT REFERENCE
	* =============================================
	*/
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0; // Index of attachment in attachment list
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Specifies the attachment type this ref is reffering to

	/*
	* =============================================
	* SUBPASS
	* =============================================
	*/
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef; // This is the REFERENCE not the actual attachment. This is to have the ability to reuse attachments through a reference in different subpasses

	/*
	* =============================================
	* SUBPASS DEPENDENCIES
	* =============================================
	*/
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Source/before subpass. EXTERNAL here represents the implicit starting subpass
	dependency.dstSubpass = 0; // Destination subpass index
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Wait on color attachment image before proceeding with the next subpass
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // What we are doing next basically. The operations that are waiting on the VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// =============================================
	// NOTE
	// =============================================
	// The layout(location = 0) out vec4 outColor directive in the GLSL fragment shader code is an output variable declaration.
	// It means that outColor is an output from the fragment shader and its output location is 0. 
	// This location directly corresponds to the index of the attachment in the subpass description.


	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CALL(vkCreateRenderPass(m_device.getLogicalDevice(), &renderPassInfo, nullptr, &m_renderPass), "Failed to create Renderpass!");
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



