#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "wyvk_commandpool.h"

namespace Wyvern {

class WYVKCommandBuffer 
{
public:
	WYVKCommandBuffer(WYVKDevice& device, WYVKCommandPool& commandPool, VkCommandBufferLevel level, uint32_t count);
	void destroy();

	void startRecording();
	void stopRecording();
	void reset();

	void submit();

	inline VkCommandBuffer* getCommandBuffer() { return &m_commandBuffer; }

private:
	VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;

	// Handles
	WYVKCommandPool& m_commandPool;
	WYVKDevice& m_device;
};


}