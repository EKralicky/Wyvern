#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "wyvk_shader.h"
#include "../wyvk_swapchain.h"
#include "wyvk_renderpass.h"

#include <filesystem>


namespace Wyvern {

class WYVKGraphicsPipeline
{
public:
	struct PipelineConfigInfo {
		VkViewport viewport{};
		VkRect2D scissor{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		VkPipelineViewportStateCreateInfo viewportStateInfo{};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};

		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
	};

	WYVKGraphicsPipeline(WYVKDevice& device, WYVKSwapchain& swapchain, WYVKRenderPass& renderPass);
	void destroy();

	void createGraphicsPipeline();
	void initializeDynamicStates(std::vector<VkDynamicState>& dynamicStates);
	void initializeDefaultPipelineInfo();

	inline VkPipeline& getPipeline() { return m_graphicsPipeline; }
	inline WYVKRenderPass& getWYVKRenderPass() { return m_renderPass; }

private:
	void configureShader(std::filesystem::path path, VkShaderStageFlagBits shaderStage);

	std::vector<std::pair<VkShaderStageFlagBits, VkShaderModule>> m_shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
	PipelineConfigInfo m_configInfo;
	VkPipelineLayout m_pipelineLayout = nullptr;
	VkPipeline m_graphicsPipeline;


	bool m_usingDynamicStates = false;

	// Handles
	WYVKDevice& m_device;
	WYVKSwapchain& m_swapchain;
	WYVKRenderPass& m_renderPass;
};

}

