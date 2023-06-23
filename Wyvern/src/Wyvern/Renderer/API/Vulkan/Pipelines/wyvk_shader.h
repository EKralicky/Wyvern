#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_device.h"
#include <shaderc/shaderc.hpp>

namespace Wyvern {

class WYVKShader
{
public:
	WYVKShader(WYVKDevice& device, std::filesystem::path& filePath, VkShaderStageFlagBits shaderStage);
	~WYVKShader();

	void compile();
	VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
	std::vector<uint32_t> getBinary() { return m_binary; }

private:
	shaderc_shader_kind mapShaderStageToKind(VkShaderStageFlagBits stage);

	std::filesystem::path m_filePath;
	VkShaderStageFlagBits m_shaderStage;
	std::vector<uint32_t> m_binary;

	// Handles
	WYVKDevice& m_device;
};

}