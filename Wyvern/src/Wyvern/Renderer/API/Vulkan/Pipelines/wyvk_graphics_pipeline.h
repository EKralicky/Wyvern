#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "wyvk_shader.h"

#include <filesystem>


namespace Wyvern {

class WYVKGraphicsPipeline
{
public:
	WYVKGraphicsPipeline(WYVKDevice& device);
	void destroy();

	void createGraphicsPipeline();

private:
	void configureShader(std::filesystem::path path, VkShaderStageFlagBits shaderStage);

	std::vector<std::pair<VkShaderStageFlagBits, VkShaderModule>> m_shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
	// Handles
	WYVKDevice& m_device;

};

}

