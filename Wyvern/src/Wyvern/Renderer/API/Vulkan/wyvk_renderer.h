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
    WYVKRenderer(Window& window);
    void destroy();

    void recordCommandBuffers();

    void beginRenderPass(WYVKCommandBuffer& commandBuffer, uint32_t imageIndex, VkClearValue& clearColor);
    void endRenderPass(WYVKCommandBuffer& commandBuffer);
    void bindPipeline(WYVKCommandBuffer& commandBuffer);
    void setDynamicPipelineStates(WYVKCommandBuffer& commandBuffer, VkViewport viewport, VkRect2D scissor);
    void draw(WYVKCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

private:

    std::unique_ptr<WYVKInstance> m_instance;
    std::unique_ptr<WYVKDevice> m_device;
    std::unique_ptr<WYVKSurface> m_surface;
    std::unique_ptr<WYVKSwapchain> m_swapchain;

    std::unique_ptr<WYVKRenderPass> m_renderPass; // might need multiple render passes and pipelines later on
    std::unique_ptr<WYVKGraphicsPipeline> m_graphicsPipeline;

    Window& m_window;
};

}



