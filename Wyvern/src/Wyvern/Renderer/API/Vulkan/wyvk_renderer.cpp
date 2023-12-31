#include "wyvk_renderer.h"
#include "Descriptor/wyvk_descriptorlayout.h"

namespace Wyvern {


WYVKRenderer::WYVKRenderer(Window& window)
    : m_window(window),
    m_instance(std::make_unique<WYVKInstance>()),
    m_device(std::make_unique<WYVKDevice>(*m_instance)),
    m_surface(std::make_unique<WYVKSurface>(*m_instance, *m_device, window)),
    m_swapchain(std::make_unique<WYVKSwapchain>(*m_instance, *m_device, *m_surface, window))
    //m_descriptorSetLayout(WYVKDescriptorSetLayout(*m_device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT))
{

    // Validate and create swapchain
    m_swapchain->validateSwapchainSupport();
    m_swapchain->createSwapchain();
    m_swapchain->createImageViews();

    // Create the render pass & store
    m_renderPass = std::make_unique<WYVKRenderPass>(*m_swapchain, *m_device);
    m_renderPass->createRenderPass();
    m_renderPass->createFramebuffers();

    // Create graphics pipeline & render frame contexts (which includes the descriptorsetlayout which is needed in the pipeline)
    createRenderFrameContexts();

    m_graphicsPipeline = std::make_unique<WYVKGraphicsPipeline>(*m_device, *m_swapchain, *m_renderPass);
    m_graphicsPipeline->createGraphicsPipeline(m_descriptorSetLayout->getLayout());


    m_commandPool = std::make_unique<WYVKCommandPool>(*m_device);   
    createCommandBuffers();

    // Create fence for transfer operations
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    VK_CALL(vkCreateFence(m_device->getLogicalDevice(), &fenceInfo, nullptr, &m_transferFence), "Unable to create transfer fence!");
}

WYVKRenderer::~WYVKRenderer()
{
    WYVERN_LOG_INFO("Destroying Renderer...");
    for (FrameContext& context : m_frameContexts) {
        destroySyncObjects(context);
    }
    // Make sure to destroy the transfer fence too
    vkDestroyFence(m_device->getLogicalDevice(), m_transferFence, nullptr);

}

void WYVKRenderer::checkGLFWSupportedExtensions(std::vector<VkExtensionProperties>& availableExtensionProperties)
{
    WYVERN_LOG_INFO("Checking support for required GLFW instance extensions...");
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        const char* currentExt = glfwExtensions[i];

        auto result = std::find_if(availableExtensionProperties.begin(),
            availableExtensionProperties.end(),
            [currentExt](VkExtensionProperties e) { return strcmp(e.extensionName, currentExt); });

        if (result != availableExtensionProperties.end()) {
            WYVERN_LOG_INFO("\tExtension: {} | Satisfied", glfwExtensions[i]);
        }
        else {
            WYVERN_LOG_INFO("\tExtension: {} | Not Found!", glfwExtensions[i]);
        }
    }
}


void WYVKRenderer::initRenderAPI()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // get extension count
    std::vector<VkExtensionProperties> extensions(extensionCount); // allocate space for extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); // retrieve available extensions

    WYVERN_LOG_INFO("Available Extensions:");
    for (const auto& extension : extensions) {
        WYVERN_LOG_INFO("\t{}", extension.extensionName);
    }
    checkGLFWSupportedExtensions(extensions);
}


bool WYVKRenderer::acquireNextSwapchainImage(uint32_t currentFrame, uint32_t& currentImage)
{
    // Waits for the In - Flight Fence bound to a specific framebuffer to be signaled. When a fence is signaled, it means the work done previously on the 
    // framebuffer is complete and we can start to render to that frame buffer.
    // This is called before we start our rendering sequence so that the CPU stalls before it knows it can start on a new frame.
    VK_CALL(vkWaitForFences(m_device->getLogicalDevice(), 1, &m_frameContexts[currentFrame].inFlightFence, VK_TRUE, UINT64_MAX), "Failed to wait for fence!");
    
    VkResult result = vkAcquireNextImageKHR(m_device->getLogicalDevice(), m_swapchain->getSwapchain(), UINT64_MAX, m_frameContexts[currentFrame].imageAvailableSemaphore, VK_NULL_HANDLE, &currentImage);

    // Returns false if the swapchain is out of date or the window was resized
    if (result == VK_ERROR_OUT_OF_DATE_KHR || m_window.isFramebufferResized()) {
        return false;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    // This call simply changes the state of a fence to be un-signaled. This is done once we know we have an image we can draw to.
    // If, for example, we un-signaled the fence right after we wait on it and the swapchain needs to be recreated, 
    // the drawFrame function will exit early, and on the next frame the fence will be waiting for a non existent frame to finish 
    VK_CALL(vkResetFences(m_device->getLogicalDevice(), 1, &m_frameContexts[currentFrame].inFlightFence), "Failed to reset fence!");
    return true;
}

void WYVKRenderer::immediateSubmit(std::function<void(VkCommandBuffer cmd)>&& func) 
{
    WYVKCommandBuffer cmdBuffer = WYVKCommandBuffer(*m_device, *m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
    cmdBuffer.startRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    func(*cmdBuffer.getCommandBuffer());

    cmdBuffer.stopRecording();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = cmdBuffer.getCommandBuffer();
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;


    VK_CALL(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit command buffer!");
    VK_CALL(vkQueueWaitIdle(m_device->getGraphicsQueue()), "Failed to wait for graphics queue during single time command!");
}

void WYVKRenderer::beginFrameRecording(uint32_t currentFrame, uint32_t currentImage)
{
    WYVKCommandBuffer* cmdBuffer = m_frameContexts[currentFrame].commandBuffer.get();
    cmdBuffer->reset();
    cmdBuffer->startRecording(0);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->getRenderPass();
    renderPassInfo.framebuffer = m_renderPass->getFrameBuffers()[currentImage];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapchain->getExtent();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(*cmdBuffer->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

}

void WYVKRenderer::endFrameRecording(uint32_t currentFrame)
{
    WYVKCommandBuffer* cmdBuffer = m_frameContexts[currentFrame].commandBuffer.get();
    vkCmdEndRenderPass(*cmdBuffer->getCommandBuffer());
    cmdBuffer->stopRecording();
}

void WYVKRenderer::setupGraphicsPipeline(uint32_t currentFrame)
{
    // Get swapchain extent
    VkExtent2D& extent = getSwapchain().getExtent();

    // Set dynamic viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = -static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    /*
    * Adjust viewport origin for flipping.
    * Otherwise setting the height to -1 will flip the viewport around 0,0 (top left)
    * and our image will be displayed off screen
    * [https://stackoverflow.com/questions/45570326/flipping-the-viewport-in-vulkan]
    */
    viewport.y += abs(viewport.height);

    vkCmdSetViewport(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), 0, 1, &viewport);

    // Set dynamic scissor
    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), 0, 1, &scissor);
}

void WYVKRenderer::createCommandBuffers()
{
    for (FrameContext& context : m_frameContexts) {
        context.commandBuffer = std::make_unique<WYVKCommandBuffer>(*m_device, *m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
    }
}

void WYVKRenderer::recreateCommandBuffers()
{
    for (FrameContext& context : m_frameContexts) {
        context.commandBuffer.reset();
    }
    createCommandBuffers();
}

void WYVKRenderer::updateUniformBuffers(uint32_t currentImage, void* data, size_t size)
{
    memcpy(m_frameContexts[currentImage].cameraMVPBuffer->getPersistentMapping(), data, size);
}

void WYVKRenderer::bindDescriptorSets(uint32_t currentFrame)
{
    VkDescriptorSet* ds = &m_frameContexts[currentFrame].descriptorSet->getDescriptorSet();
    vkCmdBindDescriptorSets(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipelineLayout(), 0, 1, ds, 0, nullptr);
}

void WYVKRenderer::bindPipeline(uint32_t currentFrame)
{
    vkCmdBindPipeline(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipeline());
}

void WYVKRenderer::draw(uint32_t currentFrame, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void WYVKRenderer::drawIndexed(uint32_t currentFrame, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    vkCmdDrawIndexed(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
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



    // ==========================================
    // DESTROY
    // ==========================================
    m_swapchain->destroy();                 // Destroy swapchain and image views
    m_renderPass->destroyImageResources();  // destroy all image resources related to the renderpass such as depth and stencil images
    m_renderPass->destroyFramebuffers();

    for (FrameContext& context : m_frameContexts) {    // Destroy all sync objects from frame contexts
        destroySyncObjects(context);
    }
    vkDestroyFence(m_device->getLogicalDevice(), m_transferFence, nullptr);    // Make sure to destroy the transfer fence too
    // ==========================================



    // ==========================================
    // CREATE
    // ==========================================
    m_swapchain->createSwapchain(); // Create swapchain and images
    m_swapchain->createImageViews(); // Create swapchain image views
    m_renderPass->createImageResources(); // Create needed renderpass images and image views
    m_renderPass->createFramebuffers(); // Recreate framebuffers based on the new image views

    for (FrameContext& context : m_frameContexts) {    // Create all sync objects from frame contexts
        createSyncObjects(context); 
    }

    VkFenceCreateInfo fenceInfo{};    // Make sure to recreate the transfer fence. There's probably a better way to do this
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    VK_CALL(vkCreateFence(m_device->getLogicalDevice(), &fenceInfo, nullptr, &m_transferFence), "Unable to create transfer fence!");
    // ==========================================

}

void WYVKRenderer::submitCommandBuffer(uint32_t currentFrame)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_frameContexts[currentFrame].imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = m_frameContexts[currentFrame].commandBuffer->getCommandBuffer();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_frameContexts[currentFrame].renderFinishedSemaphore;

    VK_CALL(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, m_frameContexts[currentFrame].inFlightFence), "Failed to submit command buffer!");
}

void WYVKRenderer::present(uint32_t currentFrame, uint32_t imageIndex)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_frameContexts[currentFrame].renderFinishedSemaphore;

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

WYVKBuffer* WYVKRenderer::createVertexBuffer(void* data, const VkDeviceSize size)
{
    WYVERN_LOG_INFO("Creating vertex buffer of size {}", size);

    WYVKBuffer* vertexBuffer = new WYVKBuffer(*m_device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_stagingBuffer->assignMemory(data);
    m_stagingBuffer->copyTo(*vertexBuffer, size, *m_commandPool, m_transferFence);

    return vertexBuffer;
}

void WYVKRenderer::bindVertexBuffers(uint32_t currentFrame, uint32_t vertexBuffersCount, VkBuffer* buffers)
{
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), 0, vertexBuffersCount, buffers, offsets);
}

WYVKBuffer* WYVKRenderer::createIndexBuffer(void* data, const VkDeviceSize size)
{
    WYVERN_LOG_INFO("Creating index buffer of size {}", size);

    WYVKBuffer* indexBuffer = new WYVKBuffer(*m_device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_stagingBuffer->assignMemory(data);
    m_stagingBuffer->copyTo(*indexBuffer, size, *m_commandPool, m_transferFence);

    return indexBuffer;
}

void WYVKRenderer::bindIndexBuffer(uint32_t currentFrame, VkBuffer buffer, VkIndexType indexType)
{
    vkCmdBindIndexBuffer(*m_frameContexts[currentFrame].commandBuffer->getCommandBuffer(), buffer, 0, indexType);
}

void WYVKRenderer::allocateStagingBuffer(VkDeviceSize size)
{
    m_stagingBuffer = std::make_unique<WYVKBuffer>(*m_device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

void WYVKRenderer::createRenderFrameContexts()
{
    // Create descriptor pool of 10 descriptors and a max of 10 sets
    m_descriptorPool = std::make_unique<WYVKDescriptorPool>(*m_device, 10, 10);
    
    // Create descriptor layout bindings and actual layout object
    m_descriptorSetLayout = std::make_unique<WYVKDescriptorLayout>(*m_device);
    m_descriptorSetLayout->addBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    m_descriptorSetLayout->createLayout();

    m_frameContexts.resize(MAX_FRAMES_IN_FLIGHT + 1);

    for (FrameContext& context : m_frameContexts) {
        createSyncObjects(context);
        initDescriptors(context);
    }
}

/*
* VERY IMPORTANT FUNCTION
*/
void WYVKRenderer::initDescriptors(FrameContext& context)
{
    context.cameraMVPBuffer = std::make_unique<WYVKBuffer>(*m_device, sizeof(CameraMVPBuffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    context.cameraMVPBuffer->createPersistentMapping();

    context.descriptorSet = std::make_unique<WYVKDescriptorSet>(*m_device, *m_descriptorPool, *m_descriptorSetLayout);
    context.descriptorSet->updateBinding(context.cameraMVPBuffer->getBuffer(), 0, sizeof(CameraMVPBuffer), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);  
}

void WYVKRenderer::createSyncObjects(FrameContext& context)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Create fence in signaled state so the fence does not block indefinitely before the first frame

    if (vkCreateSemaphore(m_device->getLogicalDevice(), &semaphoreInfo, nullptr, &context.imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(m_device->getLogicalDevice(), &semaphoreInfo, nullptr, &context.renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(m_device->getLogicalDevice(), &fenceInfo, nullptr, &context.inFlightFence) != VK_SUCCESS)
    {
        WYVERN_LOG_ERROR("Failed to create synchronization objects for a frame!");
        throw std::runtime_error("Failed to create synchronization objects for a frame!");
    }
}

void WYVKRenderer::destroySyncObjects(FrameContext& context)
{
    vkDestroySemaphore(m_device->getLogicalDevice(), context.imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(m_device->getLogicalDevice(), context.renderFinishedSemaphore, nullptr);
    vkDestroyFence(m_device->getLogicalDevice(), context.inFlightFence, nullptr);
}

}