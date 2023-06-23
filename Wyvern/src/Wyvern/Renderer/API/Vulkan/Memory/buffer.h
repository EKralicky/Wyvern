#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "../Command/wyvk_commandpool.h"
#include "../Command/wyvk_commandbuffer.h"

/*
* https://vulkan-tutorial.com/Vertex_buffers/Vertex_buffer_creation
*/

namespace Wyvern {

class WYVKBuffer
{
public:
    WYVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, WYVKDevice& device);
    ~WYVKBuffer();

    inline VkBuffer& getBuffer() { return m_buffer; }
    // Copies buffer contents from src to dst. This function is mainly used for copying info from
    // a staging buffer to a vertex buffer or other buffer
    void copyTo(VkBuffer dst, VkDeviceSize size, WYVKCommandPool commandPool);
    void copyTo(WYVKBuffer& dst, VkDeviceSize size, WYVKCommandPool commandPool);

    void assignMemory(void* srcData);
    void freeMemory();

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkDeviceSize m_size;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

    // Handles
    WYVKDevice& m_device;

};

}