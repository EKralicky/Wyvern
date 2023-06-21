#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"

/*
* https://vulkan-tutorial.com/Vertex_buffers/Vertex_buffer_creation
*/

namespace Wyvern {

class WYVKBuffer
{
public:
    WYVKBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, WYVKDevice& device);
    ~WYVKBuffer();


    inline VkBuffer& getBuffer() { return m_buffer; }

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyMemory(void* srcData);

    VkDeviceSize m_size;
    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;

    // Handles
    WYVKDevice& m_device;

};

}