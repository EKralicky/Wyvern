#include "wyvk_debug.h"

namespace Wyvern {

void WYVKMessenger::initialize(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    if (!ENABLE_VALIDATION_LAYERS) return;
    if (createDebugMessengerEXT(instance, &createInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void WYVKMessenger::destroy(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    destroyDebugMessengerEXT(instance, m_debugMessenger, pAllocator);
}

VkResult WYVKMessenger::createDebugMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, &m_debugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void WYVKMessenger::destroyDebugMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, m_debugMessenger, pAllocator);
    }
}

}


