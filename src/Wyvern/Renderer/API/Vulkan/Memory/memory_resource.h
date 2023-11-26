#pragma once

#include "Wyvern/core.h"
#include "../wyvk_device.h"

namespace Wyvern {

    class WYVKMemoryResource
    {
    public:
        WYVKMemoryResource(WYVKDevice& device, VkMemoryPropertyFlags properties);
        virtual ~WYVKMemoryResource();

    protected:
        uint32_t findMemoryType(uint32_t typeFilter);
        void createPersistentMapping();
        void allocateMemory(VkMemoryRequirements properties);
        void freeMemory();

        VkDeviceSize m_size = 0;
        VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
        void* m_mappedMemory = nullptr;
        VkMemoryPropertyFlags m_properties;


        // Handles
        WYVKDevice& m_device;
    };

} // namespace Wyvern
