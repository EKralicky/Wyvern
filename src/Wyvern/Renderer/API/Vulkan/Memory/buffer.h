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
    WYVKBuffer(WYVKDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~WYVKBuffer();

    /*
    * Copies buffer contents from src to dst. This function is mainly used for copying info from
    * a staging buffer to a vertex buffer or other buffer
    */
    void copyTo(VkBuffer dst, VkDeviceSize size, WYVKCommandPool& commandPool, VkFence transferFence);

    /*
    * Wrapper for `copyTo` to accept the WYVKBuffer type
    */
    void copyTo(WYVKBuffer& dst, VkDeviceSize size, WYVKCommandPool& commandPool, VkFence transferFence);

    /*
    * Copies data from srcData to the device memory at m_bufferMemory 
    */
    void assignMemory(void* srcData);
    
    /*
    * Frees the data in m_bufferMemory
    */
    void freeMemory();

    /*
    * Creates a persistent pointer to the device memory created by `assignMemory` so we can
    * access/update the data at any time in the application. Using a persistent map is especially useful
    * for uniform buffers where we need to update the memory every frame
    */
    void createPersistentMapping();


    inline void* getPersistentMapping() { 
        if (m_mappedMemory != nullptr) {
            return m_mappedMemory;
        }
        WYVERN_LOG_ERROR("Unable to access uninitialized persistent mapping. m_mappedMemory is nullptr!\nMake sure you created a persistentMapping using `createPersistentMap()`");
        WYVERN_THROW("Unable to access uninitialized persistent mapping!");
    }

    inline VkBuffer& getBuffer() { 
        if (m_buffer != VK_NULL_HANDLE) {
            return m_buffer;
        }
        WYVERN_LOG_ERROR("Unable to access uninitialized buffer handle. m_buffer is VK_NULL_HANDLE!\nMake sure you assigned memory to this buffer using `assignMemory(void*)`");
        WYVERN_THROW("Unable to access uninitialized buffer handle!");
    }

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkDeviceSize m_size;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
    void* m_mappedMemory = nullptr; // Persistent map: Pointer to memory that can be accessed every frame by the CPU.

    // Handles
    WYVKDevice& m_device;

};

}