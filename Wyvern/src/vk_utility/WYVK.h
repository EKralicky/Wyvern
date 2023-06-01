#pragma once
#include <vector>
#include <stdexcept>

#include "Wyvern/Core.h"

namespace Wyvern {


static std::vector<const char*> WYVKValidationLayers {
		"VK_LAYER_KHRONOS_validation"
};

static std::vector<const char*> WYVKEnabledExtensions;

	// Enable validation layers if in debug mode
#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif

#define VK_CALL(x) if (x != VK_SUCCESS) throw std::runtime_error("VK call failed!");


class WYVK 
{
public:

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		WYVERN_LOG_RENDER_API("validation layer: {}", pCallbackData->pMessage);
		return VK_FALSE;
	}

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
	};

	// Instance creation
	static void createInstance(VkInstance& instance, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);

	// Debug messenger
	static void initDebugMessenger(VkInstance& instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo, VkDebugUtilsMessengerEXT& debugMessenger);
	static VkResult createDebugMessengerEXT(VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static void destroyDebugMessengerEXT(VkInstance& instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	// Extensions & Validation layers
	static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
	static void getRequiredExtensions(std::vector<const char*>& extensions);

	// Info creation
	static void createDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static void createAppInfo(VkApplicationInfo& createInfo);

	//Device
	static std::vector<VkPhysicalDevice> queryPhysicalDevices(VkInstance& instance);
	static void pickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& physicalDevice);
	static bool rateDeviceSuitability(const VkPhysicalDevice& device);
	static QueueFamilyIndices findQueueFamilies();
private:
};

} 
