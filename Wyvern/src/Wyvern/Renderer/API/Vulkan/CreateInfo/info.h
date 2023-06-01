#pragma once
#include "Wyvern/core.h"

namespace Wyvern {
    namespace VKInfo {
        void createDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
        void createAppInfo(VkApplicationInfo& createInfo);
        void createDeviceInfo(VkDeviceCreateInfo& createInfo, VkDeviceQueueCreateInfo& queueCreateInfo, VkPhysicalDeviceFeatures& deviceFeatures, std::vector<const char*>& validationLayers);
        void createDeviceQueueInfo(VkDeviceQueueCreateInfo& createInfo, int queueFamilyIndex, int queueCount, float* queuePriority);
    }
}