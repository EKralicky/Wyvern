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

void Application::drawFrame()
{
	uint32_t imageIndex = 0; // Current swapchain image index we are drawing to

	// Wait for the fences from the last loop of this frame index
	// Since we started our fence in the signaled state, this will not block
	m_renderer->waitForFences(m_currentFrame);

	// Aquire next image from the swap chain & store index in imageIndex
	VkResult result = m_renderer->aquireNextSwapchainImage(m_currentFrame, imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || m_window->isFramebufferResized()) {
		m_window->setFramebufferResized(false);
		m_renderer->recreateSwapchain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	// Reset fences only if we have an image from the swapchain to process
	// This is to avoid deadlock if we reset the swapchain in the above codes
	m_renderer->resetFences(m_currentFrame);

	WYVKCommandBuffer* cmdBuffer = m_renderer->getCommandBuffers()[m_currentFrame].get();
	// RECORDING START
	cmdBuffer->reset();
	cmdBuffer->startRecording();
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	m_renderer->beginRenderPass(cmdBuffer, imageIndex, clearColor);

	// Get swapchain extent
	VkExtent2D& extent = m_renderer->getSwapchain().getExtent();

	// Set dynamic viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	m_renderer->setViewport(cmdBuffer, viewport);

	// Set dynamic scissor
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	m_renderer->setScissor(cmdBuffer, scissor);

	m_renderer->bindPipeline(cmdBuffer);
	m_renderer->draw(cmdBuffer, 3, 1, 0, 0);
	m_renderer->endRenderPass(cmdBuffer);
	cmdBuffer->stopRecording();
	// RECORDING STOP

	m_renderer->submitCommandBuffer(cmdBuffer, m_currentFrame);
	m_renderer->present(m_currentFrame, imageIndex);

	m_currentFrame = (m_currentFrame + 1) % WYVKRenderer::MAX_FRAMES_IN_FLIGHT;
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(m_window->getNativeWindow())) {
		glfwPollEvents(); // Poll for events e.g. Button presses, mouse movements, window close
		drawFrame();
	}
	VK_CALL(vkDeviceWaitIdle(m_renderer->getDevice().getLogicalDevice()), "DeviceWaitIdle Failed!");
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
