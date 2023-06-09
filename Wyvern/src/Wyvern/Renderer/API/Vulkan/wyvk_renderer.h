#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include "Wyvern/window.h"

#include "wyvk_device.h"
#include "wyvk_swapchain.h"
#include "wyvk_surface.h"
#include "wyvk_instance.h"
#include "Pipelines/wyvk_graphics_pipeline.h"
#include "Command/wyvk_commandpool.h"
#include "Command/wyvk_commandbuffer.h"

namespace Wyvern {

class WYVKRenderer
{
public:
    static const int MAX_FRAMES_IN_FLIGHT = 2; // "In-Flight" meaning being rendered/prepared


    WYVKRenderer(Window& window);
    void destroy();

    void allocateCommandBuffers();
    void beginRenderPass(WYVKCommandBuffer* commandBuffer, uint32_t imageIndex, VkClearValue& clearColor);
    void endRenderPass(WYVKCommandBuffer* commandBuffer);

    // Command buffer commands
    void bindPipeline(WYVKCommandBuffer* commandBuffer);
    void setDynamicPipelineStates(WYVKCommandBuffer* commandBuffer, VkViewport viewport, VkRect2D scissor);
    void draw(WYVKCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void setViewport(WYVKCommandBuffer* commandBuffer, VkViewport& viewport);
    void setScissor(WYVKCommandBuffer* commandBuffer, VkRect2D& scissor);
    
    // Synchronization
    void createSyncObjects();
    void waitForFences(uint32_t currentFrame);

    uint32_t aquireNextSwapchainImage(uint32_t currentFrame);
    void submitCommandBuffer(WYVKCommandBuffer* commandBuffer, uint32_t currentFrame);
    void present(uint32_t currentFrame, uint32_t imageIndex);

    inline std::vector<std::unique_ptr<WYVKCommandBuffer>>& getCommandBuffers() { return m_commandBuffers; }
    inline WYVKSwapchain& getSwapchain() { return *m_swapchain; }
    inline WYVKDevice& getDevice() { return *m_device; }

private:

    std::unique_ptr<WYVKInstance> m_instance;
    std::unique_ptr<WYVKDevice> m_device;
    std::unique_ptr<WYVKSurface> m_surface;
    std::unique_ptr<WYVKSwapchain> m_swapchain;

    std::unique_ptr<WYVKRenderPass> m_renderPass; // might need multiple render passes and pipelines later on
    std::unique_ptr<WYVKGraphicsPipeline> m_graphicsPipeline;

    std::vector<std::unique_ptr<WYVKCommandBuffer>> m_commandBuffers;
    std::unique_ptr<WYVKCommandPool> m_commandPool;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    Window& m_window;
};

}



