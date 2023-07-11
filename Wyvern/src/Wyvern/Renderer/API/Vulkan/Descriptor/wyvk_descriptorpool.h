#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"

namespace Wyvern {

class WYVKDescriptorPool
{
public:
	WYVKDescriptorPool(WYVKDevice& device, uint32_t descriptorCount, uint32_t maxDescriptorSets);
	~WYVKDescriptorPool();

	inline VkDescriptorPool& getDescriptorPool() { return m_descriptorPool; }

private:
	VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

	// Handles
	WYVKDevice& m_device;
};


}
