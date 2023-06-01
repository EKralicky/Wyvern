#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Wyvern/Log.h"
#include "vk_utility/wyvk.h"

// Wyvern Macros
#define WYV_DEBUG_BREAK __debugbreak()
#define WYVERN_ASSERT(assert_on, msg) if (!assert_on) { WYVERN_LOG_ERROR("Assertion Failed: {}", msg); WYV_DEBUG_BREAK; }

// Vulkan macros
#define VK_CALL(x) if (x != VK_SUCCESS) throw std::runtime_error("VK call failed!");

#ifdef NDEBUG
	#define ENABLE_VALIDATION_LAYERS false
#else
	#define ENABLE_VALIDATION_LAYERS true
#endif