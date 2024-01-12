#include "resource_allocator.h"

namespace Wyvern {

ResourceAllocator::ResourceAllocator(WYVKDevice& device, WYVKInstance& instance)
{
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = device.getPhysicalDevice();
    allocatorCreateInfo.device = device.getLogicalDevice();
    allocatorCreateInfo.instance = instance.getInstance();

    vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);
}

ResourceAllocator::~ResourceAllocator()
{
    vmaDestroyAllocator(m_allocator);
}

ResourceAllocator::Buffer ResourceAllocator::createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, bool persistent)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.size = allocSize;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo vmaAllocInfo = {};
    vmaAllocInfo.usage = memUsage;
    if (persistent) {
        vmaAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }
    
    ResourceAllocator::Buffer newBuffer;
    VK_CALL(vmaCreateBuffer(m_allocator, &bufferInfo, &vmaAllocInfo, &newBuffer.buffer, &newBuffer.allocation, &newBuffer.info), "Failed to allocate Buffer!");
    return newBuffer;
}

ResourceAllocator::Buffer ResourceAllocator::createBuffer(void* srcData, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, bool persistent)
{
    ResourceAllocator::Buffer newBuffer = createBuffer(allocSize, usage, memUsage, persistent);
    if (persistent) {
        memcpy(newBuffer.info.pMappedData, srcData, allocSize);
    }
    else {
        void* mappedData;
        vmaMapMemory(m_allocator, newBuffer.allocation, &mappedData);
        memcpy(mappedData, srcData, allocSize);
        vmaUnmapMemory(m_allocator, newBuffer.allocation);
    }
    return newBuffer;
}

void ResourceAllocator::destroyBuffer(ResourceAllocator::Buffer& buffer)
{
    vmaDestroyBuffer(m_allocator, buffer.buffer, buffer.allocation);
}

}
