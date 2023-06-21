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
    // "In-Flight" meaning currently being rendered/prepared.
    // Since we are using the mailbox/triple buffering method for rendering we will have at most 2 frames in flight
    static const int MAX_FRAMES_IN_FLIGHT = 2;

    WYVKRenderer(Window& window);
    void destroy();

    /*
    * Allocates memory and creates the command buffers to be submitted in the device's graphics queue
    * The number of command buffers to be created matches the number of frames in flight;
    * so for a triple buffering application, 2 command buffers will be created.
    */
    void createCommandBuffers();

    /*
    * Recreates the command buffers created by createCommandBuffers(). We need to recreate the command buffers when recreating the swapchain
    * as using existing command buffers could lead to undefined behavior. When we recreate the swapchain we may exit the drawFrame() function
    * early resulting in outdated command buffers. We need to recreate them to resolve any potential issues that could arise.
    */
    void recreateCommandBuffers();

    /*
    * Creates synchronization objects for frame rendering. We create 2 semaphores and one fence for each command buffer:
    * 
    * Image Available Semaphore: This semaphore signifies when an image is primed for the rendering process. During command buffer
    * submission to the device's graphics queue, it's crucial to ensure that the image is fully prepared for rendering. This semaphore
    * provides the necessary synchronization by signaling the readiness of an image.
    *
    * Render Finished Semaphore: Following the completion of the rendering process, this semaphore signals that the image is ready to
    * be presented. It acts as an indicator confirming the safe transition of the image from the rendering stage to the presentation stage.
    *
    * In-Flight Fence: After the rendering and presentation of a frame in a framebuffer, the CPU needs to be notified before it can start on the next frame in that same framebuffer. 
    * Since we are dispatching commands to the GPU, the CPU doesn't know when the frame will be completed since all of the GPU work is done asynchronously. 
    * A fence is used to block the CPU before we can begin rendering the next frame.
    * 
    */
    void createSyncObjects();

    /*
    * Destroys and recreates the sync objects that were created above. Like the command buffers, this function is used when we recreate the swapchain. This is because
    * if we need a swapchain recreation, we may exit the process of rendering a frame early which could lead to a semaphore being stuck in the signaled state. We need to reset these
    * semaphores before we begin rendering again.
    */
    void recreateSyncObjects();

    /*
    * Waits for the In-Flight Fence bound to a specific framebuffer to be signaled. When a fence is signaled, it means the work done previously on the framebuffer is complete and we
    * can start to render to that frame buffer. 
    *
    * This function is called before we start our rendering sequence so that the CPU stalls before it knows it can start on a new frame.
    */
    void waitForFences(uint32_t currentFrame);

    /*
    * This function simply changes the state of a fence to be un-signaled. This is done once we know we have an image we can draw to. If, for example, we un-signaled the fence right after we wait on it
    * and the swapchain needs to be recreated, the drawFrame function will exit early and on the next frame, the fence will be waiting for a frame that doesn't exist. 
    */
    void resetFences(uint32_t currentFrame);

    //===================================
    // Command buffer commands
    //===================================

    /*
    * Command to start the render pass
    */
    void beginRenderPass(WYVKCommandBuffer* commandBuffer, uint32_t imageIndex, VkClearValue& clearColor);
    void endRenderPass(WYVKCommandBuffer* commandBuffer);
    void bindPipeline(WYVKCommandBuffer* commandBuffer);
    void setDynamicPipelineStates(WYVKCommandBuffer* commandBuffer, VkViewport viewport, VkRect2D scissor);
    void draw(WYVKCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void setViewport(WYVKCommandBuffer* commandBuffer, VkViewport& viewport);
    void setScissor(WYVKCommandBuffer* commandBuffer, VkRect2D& scissor);
    


    [[nodiscard]] VkResult acquireNextSwapchainImage(uint32_t currentFrame, uint32_t& imageIndex);
    void recreateSwapchain();

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



