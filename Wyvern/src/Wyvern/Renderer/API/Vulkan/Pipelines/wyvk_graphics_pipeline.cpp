#include "wyvk_graphics_pipeline.h"
#include "wyvk_renderpass.h"
#include "../Geometry/vertex_geometry.h"

namespace Wyvern {

const std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };


WYVKGraphicsPipeline::WYVKGraphicsPipeline(WYVKDevice& device, WYVKSwapchain& swapchain, WYVKRenderPass& renderPass)
    : m_device(device),
    m_swapchain(swapchain),
    m_renderPass(renderPass)
{
}

WYVKGraphicsPipeline::~WYVKGraphicsPipeline()
{
    vkDestroyPipeline(m_device.getLogicalDevice(), m_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_device.getLogicalDevice(), m_pipelineLayout, nullptr);

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

void WYVKGraphicsPipeline::createShaderStates()
{
    std::filesystem::path vertexShaderPath("src\\Wyvern\\Assets\\Shaders\\vertex.vert");
    std::filesystem::path fragmentShaderPath("src\\Wyvern\\Assets\\Shaders\\fragment.frag");

    configureShader(std::filesystem::absolute(vertexShaderPath), VK_SHADER_STAGE_VERTEX_BIT);
    configureShader(std::filesystem::absolute(fragmentShaderPath), VK_SHADER_STAGE_FRAGMENT_BIT);

    WYVERN_LOG_INFO("Creating {} shader modules", m_shaderModules.size());
    for (const auto& shaderModule : m_shaderModules) {
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = shaderModule.first;
        shaderStageInfo.module = shaderModule.second;
        shaderStageInfo.pName = "main";

        m_shaderStages.push_back(shaderStageInfo);
    }
}

void WYVKGraphicsPipeline::createGraphicsPipeline(VkDescriptorSetLayout& descriptorSetLayout)
{
    createShaderStates();
    initializeDynamicStates(dynamicStates);
    initializeDefaultPipelineInfo();
    createPipelineLayoutInfo(descriptorSetLayout);

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    m_configInfo.vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_configInfo.vertexInputInfo.vertexBindingDescriptionCount = 1;
    m_configInfo.vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
    m_configInfo.vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    m_configInfo.vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

    // Create Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = m_shaderStages.data();
    pipelineInfo.pVertexInputState = &m_configInfo.vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &m_configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &m_configInfo.viewportStateInfo;
    pipelineInfo.pRasterizationState = &m_configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &m_configInfo.multisampleInfo;
    pipelineInfo.pDepthStencilState = &m_configInfo.depthStencilInfo; // Optional
    pipelineInfo.pColorBlendState = &m_configInfo.colorBlendInfo;
    pipelineInfo.pDynamicState = &m_configInfo.dynamicStateInfo;

    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass.getRenderPass();
    pipelineInfo.subpass = 0; // index of first subpass

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    VK_CALL(vkCreateGraphicsPipelines(m_device.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline), "Failed to create graphics pipeline!");
}

void WYVKGraphicsPipeline::createPipelineLayoutInfo(VkDescriptorSetLayout& descriptorSetLayout)
{
    // Finalize layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    VK_CALL(vkCreatePipelineLayout(m_device.getLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout), "Failed to create pipeline layout!");
}

void WYVKGraphicsPipeline::initializeDynamicStates(const std::vector<VkDynamicState>& dynamicStates)
{
    m_usingDynamicStates = true;

    m_configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    m_configInfo.dynamicStateInfo.pDynamicStates = dynamicStates.data();
}

void WYVKGraphicsPipeline::initializeDefaultPipelineInfo()
{

    m_configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    m_configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    if (!m_usingDynamicStates) {
        m_configInfo.viewport.x = 0.0f;
        m_configInfo.viewport.y = 0.0f;
        m_configInfo.viewport.width = (float) m_swapchain.getExtent().width;
        m_configInfo.viewport.height = (float)m_swapchain.getExtent().height;
        m_configInfo.viewport.minDepth = 0.0f;
        m_configInfo.viewport.maxDepth = 1.0f;

        m_configInfo.scissor.offset = { 0, 0 };
        m_configInfo.scissor.extent = m_swapchain.getExtent();
    }

    m_configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;  // fragments beyond near and far plane are clamped to them
    m_configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; // Disables rasterization to the framebuffer. we want to set this to false for rasterization. not sure about raytracing though
    m_configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL; // How the rasterizer draws the triangles. e.g. Fill, line, point
    m_configInfo.rasterizationInfo.lineWidth = 1.0f;
    // culls back face geomtry. The orientation of the geometry is specified in the following parameter
    m_configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    // Specifies the orientation of vertices to NOT cull. So "front" facing geometry with clockwise vertices will be shown, 
    // while "back" facing geometry with counter-clockwise vertices will be culled
    m_configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    m_configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    m_configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
    m_configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
    m_configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

    // MSAA
    m_configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    m_configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    m_configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
    m_configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
    m_configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    m_configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

    m_configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    m_configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    m_configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    m_configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    m_configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    m_configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    m_configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    m_configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    // Alpha blending
    //m_configInfo.colorBlendAttachment.blendEnable = VK_TRUE;
    //m_configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    //m_configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    //m_configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    //m_configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    //m_configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    //m_configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    m_configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    m_configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    m_configInfo.colorBlendInfo.attachmentCount = 1;
    m_configInfo.colorBlendInfo.pAttachments = &m_configInfo.colorBlendAttachment;
    m_configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    m_configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    m_configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    m_configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

    m_configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    m_configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    m_configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    m_configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    m_configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    m_configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    m_configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    m_configInfo.depthStencilInfo.front = {};  // Optional
    m_configInfo.depthStencilInfo.back = {};   // Optional

    m_configInfo.viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_configInfo.viewportStateInfo.viewportCount = 1;
    m_configInfo.viewportStateInfo.pViewports = nullptr; // Using dynamic viewport
    m_configInfo.viewportStateInfo.scissorCount = 1;
    m_configInfo.viewportStateInfo.pScissors = nullptr; // Using dynamic scissor
}
}