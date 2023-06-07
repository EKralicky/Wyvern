#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include "Wyvern/window.h"

#include "wyvk_device.h"
#include "wyvk_swapchain.h"
#include "wyvk_surface.h"
#include "wyvk_instance.h"
#include "Pipelines/wyvk_graphics_pipeline.h"

namespace Wyvern {

class WYVKRenderer
{
public:
    WYVKRenderer(Window& window);
    void destroy();

private:

    std::unique_ptr<WYVKInstance> m_instance;
    std::unique_ptr<WYVKDevice> m_device;
    std::unique_ptr<WYVKSurface> m_surface;
    std::unique_ptr<WYVKSwapchain> m_swapchain;

    std::unique_ptr<WYVKGraphicsPipeline> m_graphicsPipeline;

    Window& m_window;
};

}



