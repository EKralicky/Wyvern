#include "wyvk_commandpool.h"

namespace Wyvern {

WYVKCommandPool::WYVKCommandPool(WYVKDevice& device)
	: m_device(device)
{
	WYVKDevice::QueueFamilyIndices queueFamilyIndices = m_device.getQueueFamilyIndices();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	// VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are rerecorded with new commands very often(may change memory allocation behavior)
	// VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); // Queue to write to
	VK_CALL(vkCreateCommandPool(m_device.getLogicalDevice(), &poolInfo, nullptr, &m_commandPool), "Failed to create command pool!");
}

WYVKCommandPool::~WYVKCommandPool()
{
	vkDestroyCommandPool(m_device.getLogicalDevice(), m_commandPool, nullptr);
}

}


