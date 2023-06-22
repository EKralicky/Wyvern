#include "wyvk_commandbuffer.h"


namespace Wyvern {

WYVKCommandBuffer::WYVKCommandBuffer(WYVKDevice& device, WYVKCommandPool& commandPool, VkCommandBufferLevel level, uint32_t count)
	: m_commandPool(commandPool),
	m_device(device)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool.getCommandPool();

	// VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers.
	// VK_COMMAND_BUFFER_LEVEL_SECONDARY : Cannot be submitted directly, but can be called from primary command buffers.
	allocInfo.level = level;
	allocInfo.commandBufferCount = count;
	VK_CALL(vkAllocateCommandBuffers(m_device.getLogicalDevice(), &allocInfo, &m_commandBuffer), "Failed to allocate command buffers!");
}

void WYVKCommandBuffer::destroy()
{
	vkFreeCommandBuffers(m_device.getLogicalDevice(), m_commandPool.getCommandPool(), 1, &m_commandBuffer);
}

void WYVKCommandBuffer::startRecording(VkCommandBufferUsageFlags flags)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
	// VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT : This is a secondary command buffer that will be entirely within a single render pass.
	// VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT : The command buffer can be resubmitted while it is also already pending execution.
	beginInfo.flags = flags; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	// Implicitly resets command buffer and all commands within it
	VK_CALL(vkBeginCommandBuffer(m_commandBuffer, &beginInfo), "Failed to start command buffer recording!");
}

void WYVKCommandBuffer::stopRecording()
{
	VK_CALL(vkEndCommandBuffer(m_commandBuffer), "Failed to record command buffer!");
}

void WYVKCommandBuffer::reset()
{
	vkResetCommandBuffer(m_commandBuffer, 0);
}

}

