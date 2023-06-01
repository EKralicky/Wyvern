#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_debug.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_device.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_swapchain.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_surface.h"

namespace Wyvern {

class WYVKRenderer
{
public:
    void init();
    void destroy();

private:
    void createInstance(VkInstance& instance, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);

    bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
    void getRequiredExtensions(std::vector<const char*>& extensions);

    VkInstance m_instance;
    VkQueue m_graphicsQueue;
    WYVKDevice m_device;
    WYVKMessenger m_debugMessenger;

    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_enabledExtensions;
};

}



