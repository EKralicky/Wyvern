#include "wyvk_descriptorset.h"

namespace Wyvern {

WYVKDescriptorSet::WYVKDescriptorSet(WYVKDevice& device, WYVKDescriptorPool& pool, WYVKDescriptorLayout& layout)
	: m_device(device)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.pNext = nullptr;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool.getDescriptorPool();
    //only 1 descriptor
    allocInfo.descriptorSetCount = 1;
    //using the global data layout
    allocInfo.pSetLayouts = &layout.getLayout();

    VK_CALL(vkAllocateDescriptorSets(m_device.getLogicalDevice(), &allocInfo, &m_descriptorSet), "failed to allocate descriptor sets!");

}

WYVKDescriptorSet::~WYVKDescriptorSet()
{
	// No desctructor since the descriptor sets get destroyed once the pool gets destroyed
}

/*
* bindBuffer = the buffer that you want to bind the descriptor to
* dstBinding = the descriptor binding within the set.
* range = the size of the bindBuffer
* type = the type of the descriptor
* 
* Essentially the specified buffer gets bound to the descriptor at the dstBinding index within the set
*/
void WYVKDescriptorSet::updateBinding(VkBuffer& bindBuffer, uint32_t dstBinding, VkDeviceSize range, VkDescriptorType type)
{
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = bindBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = range;

	VkWriteDescriptorSet setWrite{};
	setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrite.pNext = nullptr;
	setWrite.dstBinding = dstBinding;
	//of the global descriptor
	setWrite.dstSet = m_descriptorSet;
	setWrite.descriptorCount = 1;
	setWrite.descriptorType = type;
	setWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(m_device.getLogicalDevice(), 1, &setWrite, 0, nullptr);
}

}


