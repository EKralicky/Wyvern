#include "wyvk_renderer.h"

namespace Wyvern {

WYVKRenderer::WYVKRenderer(Window& window)
    : m_window(window),
    m_instance(std::make_unique<WYVKInstance>()),
    m_device(std::make_unique<WYVKDevice>(*m_instance)),
    m_surface(std::make_unique<WYVKSurface>(*m_instance, *m_device, window)),
    m_swapchain(std::make_unique<WYVKSwapchain>(*m_instance, *m_device, *m_surface, window))
{
    // Validate and create swapchain
    m_swapchain->validateSwapchainSupport();
    m_swapchain->createSwapchain();

    // Create the render pass & store
    m_renderPass = std::make_unique<WYVKRenderPass>(*m_swapchain, *m_device);
    m_renderPass->createRenderPass();

    // Create graphics pipeline
    m_graphicsPipeline = std::make_unique<WYVKGraphicsPipeline>(*m_device, *m_swapchain, *m_renderPass);
    m_graphicsPipeline->createGraphicsPipeline();

    // Create framebuffers from swapchain image views
    m_swapchain->createFrameBuffers(m_renderPass->getRenderPass());
}

void WYVKRenderer::destroy()
{
    m_renderPass->destroy();
    m_graphicsPipeline->destroy();
    m_swapchain->destroy();
    m_device->destroy();
    m_surface->destroy();
}

}