#pragma once
#include <optional>

#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_instance.h"

namespace Wyvern {

class WYVKDevice
{
public:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool hasAllValidFamilies() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

public:
	WYVKDevice(WYVKInstance& instance);
	~WYVKDevice();

	void createPhysicalDevice();
	void createLogicalDevice();

	inline VkPhysicalDevice getPhysicalDevice() const {
		WYVERN_ASSERT((m_physicalDevice != VK_NULL_HANDLE), "Failed to retrieve physical device. m_device is VK_NULL_HANDLE!");
		return m_physicalDevice;
	}

	inline VkDevice getLogicalDevice() const {
		WYVERN_ASSERT((m_logicalDevice != VK_NULL_HANDLE), "Failed to retrieve logical device. m_device is VK_NULL_HANDLE!");
		return m_logicalDevice;
	}

	inline QueueFamilyIndices getQueueFamilyIndices() const {
		return m_queueFamilyIndices;
	}

	inline VkQueue getGraphicsQueue() { return m_graphicsQueue; }
	inline VkQueue getPresentQueue() { return m_presentQueue; }

private:
	std::vector<VkPhysicalDevice> queryPhysicalDevices(VkInstance instance);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	bool ratePhysicalDeviceSuitability(VkPhysicalDevice device);
	bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
	bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device);

	const std::vector<const char*> m_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	QueueFamilyIndices m_queueFamilyIndices;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicalDevice = VK_NULL_HANDLE;
	
	// Queues
	VkQueue m_presentQueue = VK_NULL_HANDLE;
	VkQueue m_graphicsQueue = VK_NULL_HANDLE;

	//Handles
	WYVKInstance& m_instance;	
};

}
