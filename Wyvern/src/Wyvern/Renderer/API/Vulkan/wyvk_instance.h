#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/CreateInfo/info.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_debug.h"

namespace Wyvern {

class WYVKInstance
{
public:
	WYVKInstance();
	void destroy();

	bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
	void getRequiredExtensions(std::vector<const char*>& extensions);

	VkInstance getInstance() { return m_instance; }
	const std::vector<const char*>& getValidationLayers() { return m_validationLayers;  }
	std::vector<const char*>& getEnabledExtensions() { return m_enabledExtensions; }

private:
	VkInstance m_instance = VK_NULL_HANDLE;
	std::unique_ptr<WYVKMessenger> m_debugMessenger;

	const std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };
	std::vector<const char*> m_enabledExtensions;

};

}

