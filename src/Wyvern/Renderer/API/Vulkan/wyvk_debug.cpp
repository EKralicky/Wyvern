#include "wyvk_debug.h"

namespace Wyvern {

WYVKMessenger::WYVKMessenger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    : m_instance(instance)
{
    if (!ENABLE_VALIDATION_LAYERS) return;
    if (createDebugMessengerEXT(&createInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

WYVKMessenger::~WYVKMessenger()
{
    WYVERN_LOG_INFO("Destroying Debug Messenger...");
    destroyDebugMessengerEXT(m_debugMessenger, nullptr);
}

VkResult WYVKMessenger::createDebugMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(m_instance, pCreateInfo, pAllocator, &m_debugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void WYVKMessenger::destroyDebugMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(m_instance, m_debugMessenger, pAllocator);
    }
}

}


