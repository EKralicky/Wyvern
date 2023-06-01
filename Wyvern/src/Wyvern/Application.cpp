#include <vector>
#include "Application.h"

namespace Wyvern {

Application::Application()
{
	Log logger;
	logger.Init();
	WYVERN_LOG_INFO("Successfully initialized logger");
}

Application::~Application()
{

}

void Application::run()
{
	initNativeWindow(WIDTH, HEIGHT, "Vulkan");
	initVulkan();
	mainLoop();
	cleanup();
}

void Application::initNativeWindow(int width, int height, const char* title)
{
	WYVERN_LOG_INFO("Initializing Native window");
	glfwInit();										// Initialize GLFW library
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Do not create an opengl context since we are using VK. (GLFW was originally dessigned to do this)
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// Disable resizing
	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr); // Create window instance
}

void checkGLFWSupportedExtensions(std::vector<VkExtensionProperties>& availableExtensionProperties)
{
	WYVERN_LOG_INFO("Checking support for required GLFW instance extensions...");
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		const char* currentExt = glfwExtensions[i];

		auto result = std::find_if(availableExtensionProperties.begin(),
			availableExtensionProperties.end(),
			[currentExt](VkExtensionProperties e) { return strcmp(e.extensionName, currentExt); });

		if (result != availableExtensionProperties.end()) {
			WYVERN_LOG_INFO("\tExtension: {} | Satisfied", glfwExtensions[i]);
		}
		else {
			WYVERN_LOG_INFO("\tExtension: {} | Not Found!", glfwExtensions[i]);
		}
	}
}

/*
Vulkan API Pattern: (https://vulkan-tutorial.com)

	Pointer to struct with creation info
	Pointer to custom allocator callbacks, always nullptr in this tutorial
	Pointer to the variable that stores the handle to the new object

*/
void Application::initVulkan()
{
	/*
	Vulkan extensions are addendums to the Vulkan specification that drivers are not required to support. 
	They allow vendors to expand upon the existing API to allow the use of features that may be unique to 
	a particular vendor or generation of devices without having to retrofit the core API.
	https://www.collabora.com/news-and-blog/blog/2022/10/19/a-look-at-vulkan-extensions-in-venus/
	*/
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // get extension count
	std::vector<VkExtensionProperties> extensions(extensionCount); // allocate space for extensions
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); // retrieve available extensions

	WYVERN_LOG_INFO("Available Extensions:");
	for (const auto& extension : extensions) {
		WYVERN_LOG_INFO("\t{}", extension.extensionName);
	}
	checkGLFWSupportedExtensions(extensions);

	m_renderer.init();

	//VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
	//WYVK::createDebugMessengerInfo(debugMessengerInfo);
	//WYVK::createInstance(m_instance, debugMessengerInfo);
	//WYVK::initDebugMessenger(m_instance, debugMessengerInfo, m_debugMessenger);
	//WYVK::pickPhysicalDevice(m_instance, m_physicalDevice);
	//WYVK::createLogicalDevice(m_instance, m_physicalDevice, m_device);
	//vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents(); // Poll for events e.g. Button presses, mouse movements, window close
	}
}

void Application::cleanup()
{
	m_renderer.destroy();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

}
