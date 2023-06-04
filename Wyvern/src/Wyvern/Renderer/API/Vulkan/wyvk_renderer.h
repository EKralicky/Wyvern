#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_device.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_swapchain.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_surface.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_instance.h"
#include "Wyvern/window.h"

namespace Wyvern {

class WYVKRenderer
{
public:
    WYVKRenderer(Window& window);
    void destroy();

private:

    std::unique_ptr<WYVKInstance> m_instance;
    std::unique_ptr<WYVKSurface> m_surface;
    std::unique_ptr<WYVKDevice> m_device;

    Window& m_window;
};

}



