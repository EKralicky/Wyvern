#pragma once
#include "Wyvern/core.h"

#include <vector>

namespace Wyvern {
    namespace VKInfo {
        void createDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);
        void createAppInfo(VkApplicationInfo& createInfo);
        void createDeviceInfo(VkDeviceCreateInfo& createInfo, std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos, VkPhysicalDeviceFeatures& deviceFeatures, std::vector<const char*>& validationLayers);
        void createDeviceQueueInfo(VkDeviceQueueCreateInfo& createInfo, int queueFamilyIndex, int queueCount, float* queuePriority);
        void createWin32SurfaceInfo(VkWin32SurfaceCreateInfoKHR& createInfo, GLFWwindow* window);
    }
}