#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <string>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <array>
#include <chrono>
#include <stdexcept> 

/*
* So even though were using Vulkan which uses a right-handed NDC (Normalized device coordinate) space,
* we have to use GLM_FORCE_LEFT_HANDED because GLM does an implicit handedness swap due to how
* OpenGL handles handedness:
* 
* "While OpenGL NDC space was “left-handed,” all of the OpenGL matrix functions created right-handed matrices. That is, 
* model space and camera space were created as right-handed. This also means that the OpenGL functions that produce
* camera-to-clip-space transformations (glFrustum, glOrtho and the like) must perform a handedness swap to match OpenGL’s “left-handed” clip-space."
* [https://community.khronos.org/t/confused-when-using-glm-for-projection/108548]
*/
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_RADIANS
//#define GLM_FORCE_MESSAGES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// IMGUI
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/backends/imgui_impl_glfw.h"
#include "../vendor/imgui/backends/imgui_impl_vulkan.h"

// IMPLOT widgets for ImGui Plotting
#include "../vendor/implot/implot.h"

// Logging using spdlog
#include "Wyvern/logger.h"

// Surface creation method
#define USING_GLFW_SURFACE

// Wyvern macros
#define WYV_DEBUG_BREAK __debugbreak()
#define WYVERN_ASSERT(assert_on, msg) if (!assert_on) { WYVERN_LOG_ERROR("Assertion Failed: {}", msg); WYV_DEBUG_BREAK; }
#define WYVERN_THROW(message) throw std::runtime_error(message)

#define BIT(x) (1 << x) // Bit shift 1 by x places

// Vulkan macros
#define VK_CALL(x, msg) { \
    if (x != VK_SUCCESS) { \
        WYVERN_LOG_ERROR(std::string("VK_CALL Failed! - ") + msg); \
        throw std::runtime_error(std::string("VK_CALL Failed! - ") + msg); \
    } \
}

#ifdef NDEBUG
	#define ENABLE_VALIDATION_LAYERS false
#else
	#define ENABLE_VALIDATION_LAYERS true
#endif