#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"

namespace Wyvern {

class WYVKCommandPool
{
public:
	WYVKCommandPool(WYVKDevice& device);
	~WYVKCommandPool();

	VkCommandPool getCommandPool() { return m_commandPool; }

private:
	VkCommandPool m_commandPool = VK_NULL_HANDLE;
	
	// Handles
	WYVKDevice& m_device;
};

}