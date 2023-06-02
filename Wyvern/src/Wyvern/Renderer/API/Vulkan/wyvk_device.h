#pragma once
#include <optional>

#include "Wyvern/core.h"
#include "CreateInfo/info.h"

namespace Wyvern {

class WYVKDevice
{
public:
	void initialize(VkInstance instance, std::vector<const char*>& validationLayers, VkSurfaceKHR surface);
	void destroy();

	inline VkPhysicalDevice getPhysicalDevice() const {
		WYVERN_ASSERT((m_physicalDevice != VK_NULL_HANDLE), "Failed to retrieve physical device. m_device is VK_NULL_HANDLE!");
		return m_physicalDevice;
	}

	inline VkDevice getLogicalDevice() const {
		WYVERN_ASSERT((m_device != VK_NULL_HANDLE), "Failed to retrieve logical device. m_device is VK_NULL_HANDLE!");
		return m_device;
	}

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool hasAllValidFamilies() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

private:
	std::vector<VkPhysicalDevice> queryPhysicalDevices(VkInstance instance);
	bool rateDeviceSuitability(const VkPhysicalDevice device);
	bool isDeviceSuitable(const VkPhysicalDevice device); 
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice(VkInstance instance, std::vector<const char*>& validationLayers);

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface;
	VkQueue m_presentQueue;
};

}
