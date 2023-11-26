#pragma once
#include "Wyvern/core.h"
#include "CreateInfo/info.h"
#include <iostream>

namespace Wyvern {

class WYVKMessenger
{
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		//std::cout << pCallbackData->pMessage << "\n";
		WYVERN_LOG_RENDER_API("validation layer: {}", pCallbackData->pMessage);
		return VK_FALSE; // Do not abort callback
	}
	
	WYVKMessenger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	~WYVKMessenger();

private:
	VkResult createDebugMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
	void destroyDebugMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

	// Handles
	VkInstance m_instance;

};

}

