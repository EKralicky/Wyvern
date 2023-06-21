#include "buffer.h"

namespace Wyvern {

WYVKBuffer::WYVKBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, WYVKDevice& device)
    : m_device(device),
    m_size(size)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size; // size in bytes
    bufferInfo.usage = usage; // usage of data. e.g. for vertex data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_device.getLogicalDevice(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements; // size, alignment, usage/flags, memoryTypeBits
    vkGetBufferMemoryRequirements(m_device.getLogicalDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CALL(vkAllocateMemory(m_device.getLogicalDevice(), &allocInfo, nullptr, &m_bufferMemory), "Failed to allocate buffer memory!");
    VK_CALL(vkBindBufferMemory(m_device.getLogicalDevice(), m_buffer, m_bufferMemory, 0), "Unable to bind buffer memory!");

    copyMemory(data);
}

WYVKBuffer::~WYVKBuffer()
{
    vkDestroyBuffer(m_device.getLogicalDevice(), m_buffer, nullptr);
    vkFreeMemory(m_device.getLogicalDevice(), m_bufferMemory, nullptr);
}

void WYVKBuffer::copyMemory(void* srcData)
{
    void* deviceData; // will hold address of device memory, accessible to the application
    VK_CALL(vkMapMemory(m_device.getLogicalDevice(), m_bufferMemory, 0, m_size, 0, &deviceData), "Unable to map memory!"); // gets address of device memory and stores in deviceData
    memcpy(deviceData, srcData, (size_t) m_size); // Copy data from srcData to the device memory
    vkUnmapMemory(m_device.getLogicalDevice(), m_bufferMemory); // unmap the pointer so it is no longer accessible
}

uint32_t WYVKBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    WYVERN_LOG_ERROR("Unable to find suitable memory type for buffer generation!");
    WYVERN_ASSERT(false, "Unable to find suitable memory type for buffer generation!");
}

}


