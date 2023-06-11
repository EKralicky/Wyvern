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
    m_swapchain->createImageViews();
    m_swapchain->createFrameBuffers(m_renderPass->getRenderPass());

    m_commandPool = std::make_unique<WYVKCommandPool>(*m_device);
    createSyncObjects();
    createCommandBuffers();
}

void WYVKRenderer::destroy()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(m_device->getLogicalDevice(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device->getLogicalDevice(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device->getLogicalDevice(), m_inFlightFences[i], nullptr);
    }

    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        m_commandBuffers[i]->destroy();
    }

    m_commandPool->destroy();
    m_graphicsPipeline->destroy();
    m_renderPass->destroy();
    m_swapchain->destroy();
    m_device->destroy();
    m_surface->destroy();
}

void WYVKRenderer::createCommandBuffers()
{
    m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_commandBuffers[i] = std::make_unique<WYVKCommandBuffer>(*m_device, *m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
    }
}

void WYVKRenderer::recreateCommandBuffers()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        m_commandBuffers[i]->destroy();
    }
    createCommandBuffers();
}

void WYVKRenderer::beginRenderPass(WYVKCommandBuffer* commandBuffer, uint32_t imageIndex, VkClearValue& clearColor)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->getRenderPass();
    renderPassInfo.framebuffer = m_swapchain->getFrameBuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapchain->getExtent();
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(*commandBuffer->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void WYVKRenderer::endRenderPass(WYVKCommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(*commandBuffer->getCommandBuffer());
}

void WYVKRenderer::bindPipeline(WYVKCommandBuffer* commandBuffer)
{
    vkCmdBindPipeline(*commandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipeline());
}

void WYVKRenderer::setDynamicPipelineStates(WYVKCommandBuffer* commandBuffer, VkViewport viewport, VkRect2D scissor)
{
    vkCmdSetViewport(*commandBuffer->getCommandBuffer(), 0, 1, &viewport);
    vkCmdSetScissor(*commandBuffer->getCommandBuffer(), 0, 1, &scissor);
}

void WYVKRenderer::draw(WYVKCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(*commandBuffer->getCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void WYVKRenderer::setViewport(WYVKCommandBuffer* commandBuffer, VkViewport& viewport)
{
    vkCmdSetViewport(*commandBuffer->getCommandBuffer(), 0, 1, &viewport);
}

void WYVKRenderer::setScissor(WYVKCommandBuffer* commandBuffer, VkRect2D& scissor)
{
    vkCmdSetScissor(*commandBuffer->getCommandBuffer(), 0, 1, &scissor);
}

/*
* ok so currently my waitforcences function is waiting on the fence then immediately resetting it. but this basically depends on the last iteration of drawframe.
* what i think is happening, is that its waiting for the previous frame to finish "vkWaitForFences" then it blocks there. once its finished, the fences get immidiately
* reset ready for the next frame iteration. but the problem with that and recreating the swapchain is that if we return early and never get to rendering the frame,
* the vkWaitForFences will hang indefinitely because the fence will never get flagged because theres no work being done. The way to fix this is to delay the fence reset
* until we know that we have a frame to be rendered. If we can't get the swapchain images then the function will return, but the fence wont be blocked because
* it was never reset and its still in the flagged state
*/
VkResult WYVKRenderer::aquireNextSwapchainImage(uint32_t currentFrame, uint32_t& imageIndex)
{
    VkResult result = vkAcquireNextImageKHR(m_device->getLogicalDevice(), m_swapchain->getSwapchain(), UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    return result;
}

void WYVKRenderer::recreateSwapchain()
{
    // Pause on window minimization
    // When the window minimizes, the framebuffer size shrinks to 0 which is an invalid state
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window.getNativeWindow(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window.getNativeWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device->getLogicalDevice());
    WYVERN_LOG_INFO("Recreating swapchain");

    m_swapchain->destroy();
    
    m_swapchain->createSwapchain();
    m_swapchain->createImageViews();
    m_swapchain->createFrameBuffers(m_renderPass->getRenderPass());
    recreateSyncObjects();
    recreateCommandBuffers();
}

void WYVKRenderer::submitCommandBuffer(WYVKCommandBuffer* commandBuffer, uint32_t currentFrame)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_imageAvailableSemaphores[currentFrame];
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffer->getCommandBuffer();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_renderFinishedSemaphores[currentFrame];

    VK_CALL(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, m_inFlightFences[currentFrame]), "Failed to submit command buffer!");
}

void WYVKRenderer::present(uint32_t currentFrame, uint32_t imageIndex)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[currentFrame];

    VkSwapchainKHR swapChains[] = { m_swapchain->getSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    VkResult result = vkQueuePresentKHR(m_device->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.isFramebufferResized()) {
        m_window.setFramebufferResized(false);
        recreateSwapchain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}

void WYVKRenderer::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Create fence in signaled state so the fence does not block indefinitely before the first frame

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(m_device->getLogicalDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_device->getLogicalDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device->getLogicalDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
        {
            WYVERN_LOG_ERROR("Failed to create synchronization objects for a frame!");
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

void WYVKRenderer::recreateSyncObjects()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(m_device->getLogicalDevice(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device->getLogicalDevice(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device->getLogicalDevice(), m_inFlightFences[i], nullptr);
    }
    createSyncObjects();
}

void WYVKRenderer::waitForFences(uint32_t currentFrame)
{
    VK_CALL(vkWaitForFences(m_device->getLogicalDevice(), 1, &m_inFlightFences[currentFrame], VK_TRUE, UINT64_MAX), "Failed to wait for fence!");
}

void WYVKRenderer::resetFences(uint32_t currentFrame)
{
    VK_CALL(vkResetFences(m_device->getLogicalDevice(), 1, &m_inFlightFences[currentFrame]), "Failed to reset fence!");
}



}