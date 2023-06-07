#include "wyvk_graphics_pipeline.h"

namespace Wyvern {

WYVKGraphicsPipeline::WYVKGraphicsPipeline(WYVKDevice& device)
    : m_device(device)
{
}

void WYVKGraphicsPipeline::destroy()
{
    WYVERN_LOG_INFO("Destroying {} shader modules", m_shaderModules.size());
    for (auto& shaderModule : m_shaderModules) {
        vkDestroyShaderModule(m_device.getLogicalDevice(), shaderModule.second, nullptr);
    }
}

void WYVKGraphicsPipeline::configureShader(std::filesystem::path path, VkShaderStageFlagBits shaderStage)
{
    WYVKShader shader(m_device, path, shaderStage);
    shader.compile();
    VkShaderModule module = shader.createShaderModule(shader.getBinary());
    m_shaderModules.push_back(std::make_pair(shaderStage, module));
}

void WYVKGraphicsPipeline::createGraphicsPipeline()
{
    std::filesystem::path vertexShaderPath("src\\Wyvern\\Assets\\Shaders\\vertex.vert");
    std::filesystem::path fragmentShaderPath("src\\Wyvern\\Assets\\Shaders\\fragment.frag");

    configureShader(std::filesystem::absolute(vertexShaderPath), VK_SHADER_STAGE_VERTEX_BIT);
    configureShader(std::filesystem::absolute(fragmentShaderPath), VK_SHADER_STAGE_FRAGMENT_BIT);

    WYVERN_LOG_INFO("Creating {} shader modules", m_shaderModules.size());
    for (const auto& shaderModule : m_shaderModules) {
        VkPipelineShaderStageCreateInfo shaderStageInfo {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = shaderModule.first;
        shaderStageInfo.module = shaderModule.second;
        shaderStageInfo.pName = "main";

        m_shaderStages.push_back(shaderStageInfo);
    }

}

}
