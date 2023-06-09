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

void WYVKRenderer::recordCommandBuffers()
{
}

void WYVKRenderer::beginRenderPass(WYVKCommandBuffer& commandBuffer, uint32_t imageIndex, VkClearValue& clearColor)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->getRenderPass();
    renderPassInfo.framebuffer = m_swapchain->getFrameBuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapchain->getExtent();
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(commandBuffer.getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void WYVKRenderer::endRenderPass(WYVKCommandBuffer& commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer.getCommandBuffer());
}

void WYVKRenderer::bindPipeline(WYVKCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipeline());
}

void WYVKRenderer::setDynamicPipelineStates(WYVKCommandBuffer& commandBuffer, VkViewport viewport, VkRect2D scissor)
{
    vkCmdSetViewport(commandBuffer.getCommandBuffer(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.getCommandBuffer(), 0, 1, &scissor);
}

void WYVKRenderer::draw(WYVKCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(commandBuffer.getCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
}

}