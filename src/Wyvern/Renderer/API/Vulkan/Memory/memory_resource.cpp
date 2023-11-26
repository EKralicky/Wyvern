#include "memory_resource.h"


namespace Wyvern {

	WYVKMemoryResource::WYVKMemoryResource(WYVKDevice& device, VkMemoryPropertyFlags properties)
		: m_device(device),
		  m_properties(properties)
	{
	}

	WYVKMemoryResource::~WYVKMemoryResource()
	{
		freeMemory();
	}

	uint32_t WYVKMemoryResource::findMemoryType(uint32_t typeFilter)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & m_properties) == m_properties) {
				return i;
			}
		}

		WYVERN_LOG_ERROR("Unable to find suitable memory type for resource generation!");
		WYVERN_THROW("Unable to find suitable memory type for resource generation!");
	}

	void WYVKMemoryResource::createPersistentMapping()
	{
		VK_CALL(vkMapMemory(m_device.getLogicalDevice(), m_deviceMemory, 0, m_size, 0, &m_mappedMemory), "Unable to create persistent mapping of device memory! Make sure that m_size is properly set.");
	}

	void WYVKMemoryResource::allocateMemory(VkMemoryRequirements requirements)
	{
		// set size for future uses such as creation of a persistent mapping
		m_size = requirements.size; 

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = requirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits);

		VK_CALL(vkAllocateMemory(m_device.getLogicalDevice(), &allocInfo, nullptr, &m_deviceMemory), "Failed to allocate device memory!");
	}

	void WYVKMemoryResource::freeMemory()
	{
		vkFreeMemory(m_device.getLogicalDevice(), m_deviceMemory, nullptr);
	}

}

