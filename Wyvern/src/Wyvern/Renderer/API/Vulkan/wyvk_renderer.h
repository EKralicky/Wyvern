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

#include "Descriptor/wyvk_descriptorpool.h"
#include "Descriptor/wyvk_descriptorset.h"

#include "Memory/buffer.h"

namespace Wyvern {

class WYVKRenderer
{
public:

    struct CameraMVPBuffer {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct FrameContext {
        VkFence inFlightFence;                  // Flags once the commandBuffer has finished rendering the current frame
        VkSemaphore imageAvailableSemaphore;   // Flags when a valid image is gathered from the swapchain
        VkSemaphore renderFinishedSemaphore;   // Flags once the GPU finishes rendering the current frame

        std::unique_ptr<WYVKCommandBuffer> commandBuffer;
        std::unique_ptr<WYVKDescriptorSet> descriptorSet = nullptr;

        // Uniforms/descriptor buffers
        std::unique_ptr<WYVKBuffer> cameraMVPBuffer;
    };

    // "In-Flight" meaning currently being rendered/prepared.
    // Since we are using the mailbox/triple buffering method for rendering we will have at most 2 frames in flight
    static const int MAX_FRAMES_IN_FLIGHT = 2;
    const VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    WYVKRenderer(Window& window);
    ~WYVKRenderer();

    /*
    * Waits for previous frame to finish via fences & prepares the next swapchain image for rendering.
    * This function returns a boolean value indicating whether the swapchain needs to be recreated. If the swapchain needs to
    * be recreated, this function will return false.
    */
    [[nodiscard]] bool acquireNextSwapchainImage(uint32_t currentFrame, uint32_t& currentImage);

    /*
    * Resets the command buffer at index `currentFrame`, starts recording, and begins the render pass
    */
    void beginFrameRecording(uint32_t currentFrame, uint32_t currentImage);

    /*
    * Stops the command buffer at index `currentFrame`, and stops the render pass
    */
    void endFrameRecording(uint32_t currentFrame);

    /*
    * Creates and updates all required pipeline states and infos.
    * Currently just gets the current swapchain extent and sets up/updates the dynamic viewport and scissor states
    */
    void setupGraphicsPipeline(uint32_t currentImage);
    
    /*
    * Binds the graphics pipeline to the command buffer specified by currentFrame for use in subsequent drawing commands.
    */
    void bindPipeline(uint32_t currentFrame);

    /*
    * Issues a command to draw primitives directly from the currently bound vertex buffer.
    * When the command is executed, primitives are assembled using the current primitive topology and `vertexCount` consecutive vertex 
    * indices. The primitives are drawn `instanceCount` times starting with `firstInstance` and increasing sequentially for each instance. 
    * The assembled primitives execute the bound graphics pipeline.
    */
    void draw(uint32_t currentFrame, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    
    /*
    * Issues a command to draw primitives from the currently bound vertex buffer using the currently bound index buffer.
    * When the command is executed, primitives are assembled using the current primitive topology and `indexCount` vertices 
    * whose indices are retrieved from the index buffer. The index buffer is treated as an array of tightly packed unsigned integers of size defined 
    * by the `indexType` parameter with which the buffer was bound. See `BindvertexBuffers()` for more info.
    */
    void drawIndexed(uint32_t currentFrame, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

    /*
    * While rendering, if the window we are drawing to gets resized or minimized, or the swapchain is underperforming, we will need
    * to recreate the swapchain. This function destroys all swapchain image views and framebuffers and also destroys all synchronization objects related to it
    * such as the in flight fences and image semaphores. Once all objects are cleaned up, the swapchain and all synchronization objects are recreated
    */
    void recreateSwapchain();

    /*
    * Submits the given frame's command buffer to the graphics queue for execution.
    * For synchronization purposes, the command buffer waits on the m_imageAvailableSemaphores flagged by the swapchain when
    * an image is requested. Once the command buffer completes, it flags the m_renderFinishedSemaphores.
    */
    void submitCommandBuffer(uint32_t currentFrame);

    /*
    * Presents the given frame's image to the swapchain for display on screen.
    * Before presenting, Vulkan must wait on the m_renderFinishedSemaphores to be flagged before presenting.
    * These semaphores get flagged once the command buffer completes.
    */
    void present(uint32_t currentFrame, uint32_t imageIndex);

    /*
    * Allocates a staging buffer with the given size.
    * The staging buffer is generally used to transfer data from the CPU to the GPU.
    */
    void allocateStagingBuffer(VkDeviceSize size);

    /*
    * Creates a new vertex buffer with the given data and size.
    * The data is copied into the buffer using a staging buffer.
    */
    WYVKBuffer* createVertexBuffer(void* data, VkDeviceSize size);

    /*
    * Binds the set of vertices you want to draw. vkCmdBindVertexBuffers can be confusing as it is plural, but this is
    * not used for unique sets of vertices and their attributes. It is used for when you want to structure your vertex buffer attributes such
    * that each attribute is in a separate buffer. For example if you wanted to store your positions in BufferA and colors in bufferB.
    * If you want to draw multiple UNIQUE buffers, you would have to make a separate draw call for each unique set of buffers. Like if
    * you wanted to draw the world and the player model, you would have to bind the world vertices and draw, and then bind the player
    * model vertices then draw again.
    */
    void bindVertexBuffers(uint32_t currentFrame, uint32_t vertexBuffersCount, VkBuffer* buffers);

    /*
    * Creates a new index buffer with the given data and size.
    * The data is copied into the buffer using a staging buffer.
    */
    WYVKBuffer* createIndexBuffer(void* data, VkDeviceSize size);

    /*
    * Binds the indices to draw. These indices should be mapped to the currently bound vertices.
    * The indexType represents the data type for each of the indices.
    */
    void bindIndexBuffer(uint32_t currentFrame, VkBuffer buffer, VkIndexType indexType);

    /*
    * Updates the data in the uniform buffers used in the rendering pipeline.
    * This function should be called whenever the data used by the shaders changes, such as when the camera moves.
    * size is in bytes.
    */
    void updateUniformBuffers(uint32_t currentImage, void* data, size_t size);

    void bindDescriptorSets(uint32_t currentFrame);

    // Getters & Setters
    inline std::vector<std::unique_ptr<WYVKCommandBuffer>>& getCommandBuffers() { return m_commandBuffers; }
    inline WYVKSwapchain& getSwapchain() { return *m_swapchain; }
    inline WYVKDevice& getDevice() { return *m_device; }

private:
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

    ///*
    //* Creates and initializes all uniform buffers for usage in shaders and the rendering pipeline.
    //* The current approach is using one uniform buffer per frame, so the number of uniform buffers must be
    //* equal to MAX_FRAMES_IN_FLIGHT
    //*/
    //void createUniformBuffers();

    /*
    * Creates synchronization objects for frame rendering. We create 2 semaphores and one fence for each frame context:
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
    void createSyncObjects(FrameContext& context);

    /*
    * Properly destroys all semaphores and fences in the render context
    * Destroys all other fences and synchronization objects as well.
    * 
    * Use when recreating the sync objects, which is necessary for swapchain recreation
    */
    void destroySyncObjects(FrameContext& context);

    void initDescriptors(FrameContext& context);

    void createRenderFrameContexts();




    std::unique_ptr<WYVKInstance> m_instance;
    std::unique_ptr<WYVKDevice> m_device;
    std::unique_ptr<WYVKSurface> m_surface;
    std::unique_ptr<WYVKSwapchain> m_swapchain;
    std::unique_ptr<WYVKRenderPass> m_renderPass; // might need multiple render passes and pipelines later on
    std::unique_ptr<WYVKGraphicsPipeline> m_graphicsPipeline;

    std::unique_ptr<WYVKCommandPool> m_commandPool;
    std::vector<std::unique_ptr<WYVKCommandBuffer>> m_commandBuffers;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;

    std::vector<VkFence> m_inFlightFences;

    // Staging buffer used for vertex & index data transfer to appropriate buffers on GPU
    // Transfer fence to signal once transfer operations are complete
    std::unique_ptr<WYVKBuffer> m_stagingBuffer;
    VkFence m_transferFence;

    // Descriptor pool/layout
    std::unique_ptr<WYVKDescriptorPool> m_descriptorPool;
    std::unique_ptr<WYVKDescriptorLayout> m_descriptorSetLayout;

    // Frame context. Holds all sync data, command buffers, descriptors etc... 
    // Essentially anything that has a per frame instance.
    std::vector<FrameContext> m_frameContexts;

    // Handles
    Window& m_window;
};

}



