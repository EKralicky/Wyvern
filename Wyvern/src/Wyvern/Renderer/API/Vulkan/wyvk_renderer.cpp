#include "wyvk_renderer.h"

namespace Wyvern {

WYVKRenderer::WYVKRenderer(Window& window)
    : m_window(window),
    m_instance(std::make_unique<WYVKInstance>()),
    m_device(std::make_unique<WYVKDevice>(*m_instance)),
    m_surface(std::make_unique<WYVKSurface>(*m_instance, *m_device, window)),
    m_swapchain(std::make_unique<WYVKSwapchain>(*m_instance, *m_device, *m_surface, window))
{
    m_swapchain->validateSwapchainSupport();
    m_swapchain->createSwapchain();

    m_graphicsPipeline = std::make_unique<WYVKGraphicsPipeline>(*m_device);
    m_graphicsPipeline->createGraphicsPipeline();
}

void WYVKRenderer::destroy()
{
    m_swapchain->destroy();
    m_device->destroy();
    m_surface->destroy();

}

}