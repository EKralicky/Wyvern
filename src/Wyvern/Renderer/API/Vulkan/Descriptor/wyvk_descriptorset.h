#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "wyvk_descriptorpool.h"
#include "wyvk_descriptorlayout.h"

namespace Wyvern {

class WYVKDescriptorSet
{
public:
    WYVKDescriptorSet(WYVKDevice& device, WYVKDescriptorPool& pool, WYVKDescriptorLayout& layout);
    ~WYVKDescriptorSet();

    void updateBinding(VkBuffer& bindBuffer, uint32_t dstBinding, VkDeviceSize range, VkDescriptorType type);

    inline VkDescriptorSet& getDescriptorSet() { return m_descriptorSet; }

private:
    VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;

    // Handles
    WYVKDevice& m_device;
};




}


