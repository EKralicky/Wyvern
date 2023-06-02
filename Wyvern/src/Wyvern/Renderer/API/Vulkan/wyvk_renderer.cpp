#include "wyvk_renderer.h"

namespace Wyvern {

void WYVKRenderer::init(GLFWwindow* window)
{
    m_window = window;
    m_validationLayers.push_back("VK_LAYER_KHRONOS_validation");

    // Retrieve createInfo for messenger
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo {};
    VKInfo::createDebugMessengerInfo(messengerCreateInfo, WYVKMessenger::debugCallback);

    // Create VK instance
	createInstance(m_instance, messengerCreateInfo);
    m_debugMessenger.initialize(m_instance, messengerCreateInfo);    // Initialize debug messenger
    m_surface.initialize(m_instance, m_window);    // Initialize surface
    m_device.initialize(m_instance, m_validationLayers);    // Setup devices
}

void WYVKRenderer::destroy()
{
    if (ENABLE_VALIDATION_LAYERS) {
        m_debugMessenger.destroy(m_instance, nullptr);
    }
    m_device.destroy();
    m_surface.destroy(m_instance);
    vkDestroyInstance(m_instance, nullptr);
}

void WYVKRenderer::createInstance(VkInstance& instance, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport(m_validationLayers)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo {};
    VKInfo::createAppInfo(appInfo);

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Apply validation layers if enabled
    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // We need extensions in Vulkan to interface with the window
    getRequiredExtensions(m_enabledExtensions); // Get GLFW required extensions

    //========================
    // ADD EXTENSIONS BELOW!!!
    //========================

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_enabledExtensions.size());
    createInfo.ppEnabledExtensionNames = m_enabledExtensions.data();

    WYVERN_LOG_INFO("Enabled Extensions:");
    for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
        WYVERN_LOG_INFO("\t{}", createInfo.ppEnabledExtensionNames[i]);
    }

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &instance), "Unable to create Vulkan instance!");
}

bool WYVKRenderer::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void WYVKRenderer::getRequiredExtensions(std::vector<const char*>& extensions)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Adds the debug extension for debug messagess.
    // These messages are useful when we are using validation layers
    if (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
}

}

