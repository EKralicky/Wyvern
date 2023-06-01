#pragma once
#include "Wyvern/core.h"


namespace Wyvern {

    namespace VKInfo {

        void createDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback) {
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
            createInfo.pUserData = nullptr; // Optional
        }

        void createAppInfo(VkApplicationInfo& createInfo) {
            createInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            createInfo.pApplicationName = "Wyvern";
            createInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            createInfo.pEngineName = "No Engine";
            createInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            createInfo.apiVersion = VK_API_VERSION_1_0;
        }

        void createDeviceInfo(VkDeviceCreateInfo& createInfo, VkDeviceQueueCreateInfo& queueCreateInfo, VkPhysicalDeviceFeatures& deviceFeatures, std::vector<const char*>& validationLayers) {
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.pQueueCreateInfos = &queueCreateInfo;
            createInfo.queueCreateInfoCount = 1;
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = 0;
            createInfo.ppEnabledExtensionNames = nullptr;

            if (ENABLE_VALIDATION_LAYERS) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }
            else {
                createInfo.enabledLayerCount = 0;
            }
        }

        void createDeviceQueueInfo(VkDeviceQueueCreateInfo& createInfo, int queueFamilyIndex, int queueCount, float* queuePriority) {
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            createInfo.queueFamilyIndex = queueFamilyIndex;
            createInfo.queueCount = queueCount;
            createInfo.pQueuePriorities = queuePriority;
        }

    }
}
