#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"

namespace Wyvern {

class WYVKDescriptorLayout
{
public:
	WYVKDescriptorLayout(WYVKDevice& devic);
	~WYVKDescriptorLayout();

	void addBinding(uint32_t binding, uint32_t descriptorCount, VkDescriptorType type, VkShaderStageFlags shaderFlags);
	void createLayout();

	inline VkDescriptorSetLayout& getLayout() { return m_layout; }

private:
	VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayoutBinding> m_bindings;

	// Handles
	WYVKDevice& m_device;
};


}
