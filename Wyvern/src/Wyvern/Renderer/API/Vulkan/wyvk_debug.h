#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"

namespace Wyvern {

class WYVKMessenger
{
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		WYVERN_LOG_RENDER_API("validation layer: {}", pCallbackData->pMessage);
		return VK_FALSE; // Do not abort callback
	}
	
	WYVKMessenger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void destroy(VkInstance instance, const VkAllocationCallbacks* pAllocator);

private:
	VkResult createDebugMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
	void destroyDebugMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	VkDebugUtilsMessengerEXT m_debugMessenger;

};

}

