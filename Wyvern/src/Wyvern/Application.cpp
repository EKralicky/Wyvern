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
void Application::drawFrame(std::vector<Model>& models, bool drawIndexed)
{
	uint32_t imageIndex = 0; // Current swapchain image index we are drawing to

	// Wait for the fences from the last loop of this frame index
	// Since we started our fence in the signaled state, this will not block
	m_renderer->waitForFences(m_currentFrame);

	// Acquire next image from the swap chain & store index in imageIndex
	VkResult result = m_renderer->acquireNextSwapchainImage(m_currentFrame, imageIndex);
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
	cmdBuffer->startRecording(0);
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

	for (Model& model : models) {
		/*
		* Binds the set of vertices you want to draw. vkCmdBindVertexBuffers can be confusing as it is plural, but this is
		* not used for unique sets of vertices and their attributes. It is used for when you want to structure your vertex buffer attributes such
		* that each attribute is in a separate buffer. For example if you wanted to store your positions in BufferA and colors in bufferB.
		* If you want to draw multiple UNIQUE buffers, you would have to make a separate draw call for each unique set of buffers. Like if
		* you wanted to draw the world and the player model, you would have to bind the world vertices and draw, and then bind the player
		* model vertices then draw again.
		*/
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*cmdBuffer->getCommandBuffer(), 0, model.getVertexBuffersCount(), model.getVertexBuffer(), offsets);

		/*
		* Binds the indices to draw. These indices should be mapped to the currently bound vertices.
		* The indexType represents the date type for each of the indices.
		*/
		vkCmdBindIndexBuffer(*cmdBuffer->getCommandBuffer(), *model.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);


		if (drawIndexed) {
			vkCmdDrawIndexed(*cmdBuffer->getCommandBuffer(), static_cast<uint32_t>(model.getIndexCount()), 1, 0, 0, 0);
		}
		else {
			// Draw the vertices to the framebuffer WITHOUT INDEXING!
			m_renderer->draw(cmdBuffer, model.getVertexSize(), 1, 0, 0);
		}
	}
	
	m_renderer->endRenderPass(cmdBuffer);
	cmdBuffer->stopRecording();
	m_renderer->submitCommandBuffer(cmdBuffer, m_currentFrame);
	m_renderer->present(m_currentFrame, imageIndex);

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
		
		drawFrame(models, true); // Uses the render API to draw a single frame
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
