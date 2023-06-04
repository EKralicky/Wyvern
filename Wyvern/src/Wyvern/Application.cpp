#include <vector>
#include "Application.h"

namespace Wyvern {

Application::Application()
{
	m_logger = std::make_unique<Logger>();
	m_logger->init();
	m_window = std::make_unique<Window>("Wyvern App");

	initRenderAPI();
	m_renderer = std::make_unique<WYVKRenderer>(*m_window);
}

Application::~Application()
{
	m_renderer->destroy();
	m_window->destroy();
}

void Application::run()
{
	mainLoop();
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(m_window->getNativeWindow())) {
		glfwPollEvents(); // Poll for events e.g. Button presses, mouse movements, window close
	}
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
void Application::initRenderAPI()
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
}

}
