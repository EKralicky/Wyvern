#pragma once
#include "Wyvern/Core.h"
#include "vma/vk_mem_alloc.h"
#include "../wyvk_device.h"

namespace Wyvern {

	class ResourceAllocator
	{
	public:
		struct Buffer
		{
			VkBuffer buffer;
			VmaAllocation allocation;
			VmaAllocationInfo info;
		};

		ResourceAllocator(WYVKDevice& device, WYVKInstance& instance);
		~ResourceAllocator();

		Buffer createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, bool persistent);
		Buffer createBuffer(void* srcData, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, bool persistent);
		void destroyBuffer(Buffer& buffer);

	private:
		VmaAllocator m_allocator = VK_NULL_HANDLE;
	};


}