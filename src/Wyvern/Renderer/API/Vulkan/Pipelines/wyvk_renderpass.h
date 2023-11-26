#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "../wyvk_swapchain.h"

namespace Wyvern {

class WYVKRenderPass
{
public:
	WYVKRenderPass(WYVKSwapchain& swapchain, WYVKDevice& device);
	~WYVKRenderPass();

	/*
	* Attachment description layout:
	* ===========================================
	* 
	* attachment.format
	* - image format
	* 
	* attachment.samples
	* - sample count for multisampling
	*
	* attachment.loadOp
	* - VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
	* - VK_ATTACHMENT_LOAD_OP_CLEAR : Clear the values to a constant at the start
	* - VK_ATTACHMENT_LOAD_OP_DONT_CARE : Existing contents are undefined; we don't care about them
	* 
	* attachment.storeOp
	* - VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
	* - VK_ATTACHMENT_STORE_OP_DONT_CARE : Contents of the framebuffer will be undefined after the rendering operation
	*
	* attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	* attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	*
	* + specifies which layout the image will have before the render pass begins.
	* attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Undefined initial state. We don't care what layout the image was previously in
	* 
	* + specifies the layout to automatically transition to when the render pass finishes.
	* attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // We are presenting directly to the swapchain
	*
	* Options for finalLayout:
	* - VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
	* - VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : Images to be presented in the swap chain
	* - VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : Images to be used as destination for a memory copy operation. most likely for post processing?
	*/
	void createRenderPass();
	void createFramebuffers();
	void createImageResources();

	// Deletes all images and image views related to the renderpass attachments. For example depth and stencil attachments
	void destroyImageResources();
	void destroyFramebuffers();

	inline VkRenderPass getRenderPass() { return m_renderPass; }
	inline std::vector<VkFramebuffer>& getFrameBuffers() { return m_framebuffers; }

private:
	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	std::vector<VkSubpassDescription> subpasses;

	// Image attachments used in subpasses e.g. depth, stencil or color images
	std::unique_ptr<WYVKImage> m_depthImage;
	std::vector<VkFramebuffer> m_framebuffers;

	// Handles
	WYVKDevice& m_device;
	WYVKSwapchain& m_swapchain;
};


}