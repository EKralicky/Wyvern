#include "wyvk_descriptorlayout.h"
#include "../wyvk_device.h"

namespace Wyvern {

WYVKDescriptorLayout::WYVKDescriptorLayout(WYVKDevice& device)
    : m_device(device)
{
}

WYVKDescriptorLayout::~WYVKDescriptorLayout()
{
    vkDestroyDescriptorSetLayout(m_device.getLogicalDevice(), m_layout , nullptr);
}

void WYVKDescriptorLayout::addBinding(uint32_t binding, uint32_t descriptorCount, VkDescriptorType type, VkShaderStageFlags shaderFlags)
{
    VkDescriptorSetLayoutBinding camBufferBinding = {};
    camBufferBinding.binding = binding;
    camBufferBinding.descriptorCount = descriptorCount;
    camBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Uniform type
    camBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Used in the vertex shader

    m_bindings.push_back(camBufferBinding);
}

void WYVKDescriptorLayout::createLayout()
{
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = m_bindings.size();
    layoutInfo.pBindings = m_bindings.data();

    VK_CALL(vkCreateDescriptorSetLayout(m_device.getLogicalDevice(), &layoutInfo, nullptr, &m_layout), "Unable to create descriptor set layout!");
}

}


