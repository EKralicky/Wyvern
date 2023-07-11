#include "wyvk_descriptorpool.h"

namespace Wyvern {

WYVKDescriptorPool::WYVKDescriptorPool(WYVKDevice& device, uint32_t descriptorCount, uint32_t maxDescriptorSets)
	: m_device(device)
{
	std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount } // Holds 10 uniform buffers/descriptors! NOT descriptor sets
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = maxDescriptorSets; // Maximum amount of descriptor SETS
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	VK_CALL(vkCreateDescriptorPool(m_device.getLogicalDevice(), &pool_info, nullptr, &m_descriptorPool), "Unable to create descriptor pool!");
}

WYVKDescriptorPool::~WYVKDescriptorPool()
{
	vkDestroyDescriptorPool(m_device.getLogicalDevice(), m_descriptorPool, nullptr);
}

}