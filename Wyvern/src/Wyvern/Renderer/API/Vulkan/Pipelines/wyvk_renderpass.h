#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "../wyvk_swapchain.h"

namespace Wyvern {

class WYVKRenderPass
{
public:
	WYVKRenderPass(WYVKSwapchain& swapchain, WYVKDevice& device);
	void destroy();

	void createRenderPass();
	//VkAttachmentDescription createAttachment();
	//void addSubpass();
	
	inline VkRenderPass getRenderPass() { return m_renderPass; }

private:
	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	std::vector<VkSubpassDescription> subpasses;
	// Handles
	WYVKDevice& m_device;
	WYVKSwapchain& m_swapchain;
};


}