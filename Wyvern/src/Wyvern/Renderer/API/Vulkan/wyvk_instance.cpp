#include "wyvk_instance.h"

Wyvern::WYVKInstance::WYVKInstance()
{
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
    VKInfo::createDebugMessengerInfo(messengerCreateInfo, WYVKMessenger::debugCallback);

    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport(m_validationLayers)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    VKInfo::createAppInfo(appInfo);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Apply validation layers if enabled
    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &messengerCreateInfo;
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

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance), "Unable to create Vulkan instance!");

    // Init debug messenger
    m_debugMessenger = std::make_unique<WYVKMessenger>(m_instance, messengerCreateInfo);
}

void Wyvern::WYVKInstance::destroy()
{
    if (ENABLE_VALIDATION_LAYERS) {
        m_debugMessenger->destroy(m_instance, nullptr);
        m_debugMessenger.reset();
    }
    vkDestroyInstance(m_instance, nullptr);
}

bool Wyvern::WYVKInstance::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
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

void Wyvern::WYVKInstance::getRequiredExtensions(std::vector<const char*>& extensions)
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

