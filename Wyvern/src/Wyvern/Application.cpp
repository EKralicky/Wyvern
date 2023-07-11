#include <vector>
#include "Application.h"
#include "Renderer/API/Vulkan/Geometry/vertex_geometry.h"
#include "Renderer/API/Vulkan/Memory/buffer.h"

namespace Wyvern {

Application::Application()
{
	// Initializes the Wyvern console logger for manual logging and the
	// Renderer logger for Vulkan validation layer logging
	m_logger = std::make_unique<Logger>();
	m_logger->init();
	m_window = std::make_unique<Window>("Wyvern App");

	initRenderAPI();
	m_renderer = std::make_unique<WYVKRenderer>(*m_window);
}

Application::~Application()
{
}

void Application::run()
{
	mainLoop();
}

/*
* TODO: make it so we can draw multiple vertex buffers in one render pass. 
This will require a restructuring of the params to this function
*/
void Application::drawFrame(std::vector<Model>& models, bool drawIndexed, void* uniformData, size_t uniformSize)
{
	uint32_t currentImage = 0; // Current swapchain image index we are drawing to

	if (m_renderer->acquireNextSwapchainImage(m_currentFrame, currentImage) == false) {
		m_window->setFramebufferResized(false);
		m_renderer->recreateSwapchain();
	}

	m_renderer->beginFrameRecording(m_currentFrame, currentImage);

	m_renderer->setupGraphicsPipeline(m_currentFrame);
	m_renderer->bindPipeline(m_currentFrame);

	for (Model& model : models) {
		
		m_renderer->bindVertexBuffers(m_currentFrame, model.getVertexBuffersCount(), model.getVertexBuffer());
		m_renderer->bindIndexBuffer(m_currentFrame, *model.getIndexBuffer(), VK_INDEX_TYPE_UINT16);
		m_renderer->bindDescriptorSets(m_currentFrame);

		if (drawIndexed) {
			m_renderer->drawIndexed(m_currentFrame, static_cast<uint32_t>(model.getIndexCount()), 1, 0, 0, 0);
		}
		else {
			m_renderer->draw(m_currentFrame, model.getVertexSize(), 1, 0, 0);
		}
	}

	m_renderer->endFrameRecording(m_currentFrame);

	m_renderer->updateUniformBuffers(currentImage, uniformData, uniformSize);

	m_renderer->submitCommandBuffer(m_currentFrame);
	m_renderer->present(m_currentFrame, currentImage);
	m_currentFrame = (m_currentFrame + 1) % WYVKRenderer::MAX_FRAMES_IN_FLIGHT;
}

void Application::mainLoop()
{
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	// One time allocation of staging buffer
	m_renderer->allocateStagingBuffer(sizeof(vertices[0]) * vertices.size());

	std::vector<Model> models;
	models.emplace_back(*m_renderer, vertices, indices);

	//// Get sizes of vertex and index buffers
	//size_t vSize = sizeof(vertices[0]) * vertices.size();
	//size_t iSize = sizeof(indices[0]) * indices.size();

	//// One time allocation of staging buffer
	//m_renderer->allocateStagingBuffer(vSize); 

	//// Create vertex and index buffers
	//std::unique_ptr<WYVKBuffer> vertexBuffer = m_renderer->createVertexBuffer((void*)vertices.data(), vSize);
	//std::unique_ptr<WYVKBuffer> indexBuffer = m_renderer->createIndexBuffer((void*)indices.data(), iSize);


	/*VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
	VkDeviceSize offsets[] = { 0 };*/

	while (!glfwWindowShouldClose(m_window->getNativeWindow())) {
		glfwPollEvents(); // Poll for events e.g. Button presses, mouse movements, window close
		WYVKRenderer::CameraMVPBuffer ubo{};


		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), m_renderer->getSwapchain().getExtent().width / (float)m_renderer->getSwapchain().getExtent().height, 0.1f, 10.0f);

		drawFrame(models, true, (void*) &ubo, sizeof(ubo)); // Uses the render API to draw a single frame
	}
	// Wait for the physical device (GPU) to be idle (Not working on anything) before we quit
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
