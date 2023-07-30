#include <vector>
#include <functional>

#include "Application.h"
#include "Renderer/API/Vulkan/Geometry/vertex_geometry.h"
#include "Renderer/API/Vulkan/Memory/buffer.h"
#include "Wyvern/Input/input_manager.h"

namespace Wyvern {



Application::Application()
	: m_player(), m_camera()
{
	// Initializes the Wyvern console logger for manual logging and the
	// Renderer logger for Vulkan validation layer logging
	m_logger = std::make_unique<Logger>();
	m_logger->init();

	// Window
	m_window = std::make_unique<Window>("Wyvern App");
	m_window->initCallbacks(BIND_INTERNAL_EVENT(Application::onEvent));

	// Renderer using Vulkan
	m_renderer = std::make_unique<WYVKRenderer>(*m_window);
	m_renderer->initRenderAPI();

	// GUI & Debug stuff from ImGui
	m_imGuiHandler = std::make_unique<ImGuiHandler>(*m_window, *m_renderer);


	// Input Handling. My philosophy on the function pointers bound to keys is that they should always have no params.
	// This is because a single key press is none other than a key press. There is no other data associated it like with
	// moving your mouse. Its basically a check for "is it pressed?" and if it is, a function gets called.
	InputManager::getInstance().bindKey(
		KeyRequirement(WYVERN_KEY_SPACE, WYVERN_MOD_NONE),
		InputAction("Jump", InputType::CONTINUOUS, BIND_EXTERNAL_FUNC(Player::jump, m_player)));

}

Application::~Application()
{
	
}

void Application::run()
{
	mainLoop();
}

void Application::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>	(BIND_INTERNAL_EVENT(Application::onWindowClose));
	dispatcher.dispatch<MouseMovedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onMouseMoved,	InputManager::getInstance()));
	dispatcher.dispatch<MouseScrolledEvent>	(BIND_EXTERNAL_EVENT(InputManager::onMouseScrolled,	InputManager::getInstance()));
	dispatcher.dispatch<KeyPressedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onKeyPressed,	InputManager::getInstance()));
	dispatcher.dispatch<KeyReleasedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onKeyReleased,	InputManager::getInstance()));
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
	m_running = false;
	return true;
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
		ImGui::EndFrame();
		return;
	}

	m_renderer->beginFrameRecording(m_currentFrame, currentImage);

	m_renderer->setupGraphicsPipeline(m_currentFrame);
	m_renderer->bindPipeline(m_currentFrame);

	m_renderer->updateUniformBuffers(currentImage, uniformData, uniformSize);

	for (Model& model : models) {
		

		m_renderer->bindVertexBuffers(m_currentFrame, static_cast<uint32_t>(model.getVertexBuffersCount()), model.getVertexBuffer());
		m_renderer->bindIndexBuffer(m_currentFrame, *model.getIndexBuffer(), VK_INDEX_TYPE_UINT16);
		m_renderer->bindDescriptorSets(m_currentFrame);

		if (drawIndexed) {
			m_renderer->drawIndexed(m_currentFrame, static_cast<uint32_t>(model.getIndexCount()), 1, 0, 0, 0);
		}
		else {
			m_renderer->draw(m_currentFrame, static_cast<uint32_t>(model.getVertexSize()), 1, 0, 0);
		}
	}

	m_imGuiHandler->renderFrame(*m_renderer->getFrameContext(m_currentFrame).commandBuffer);
	m_renderer->endFrameRecording(m_currentFrame);


	m_renderer->submitCommandBuffer(m_currentFrame);
	m_renderer->present(m_currentFrame, currentImage);
	m_currentFrame = (m_currentFrame + 1) % WYVKRenderer::MAX_FRAMES_IN_FLIGHT;

}



void Application::mainLoop()
{
	while (m_running) {

		const std::vector<Vertex> vertices = {
		{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},

		{{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		// One time allocation of staging buffer. The staging buffer is used to transfer vertex data from memory to the gpu memory.
		m_renderer->allocateStagingBuffer(sizeof(vertices[0]) * vertices.size());

		std::vector<Model> models;
		models.emplace_back(*m_renderer, vertices, indices);

		while (!m_window->shouldClose()) {

			// Poll GLFW events & process received input
			m_window->pollEvents();
			InputManager::getInstance().processInput();

			// == ImGui ==
			m_imGuiHandler->newFrame();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//m_imGuiHandler->createFrameDataPlot(1000.0f / ImGui::GetIO().Framerate);
			// ===========




			// Update Camera
			WYVKRenderer::CameraMVPBuffer ubo{};
			static auto startTime = std::chrono::high_resolution_clock::now();
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			//ubo.model = glm::mat4(1);
			ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.view = glm::lookAt(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			ubo.proj = glm::perspective(glm::radians(45.0f), m_renderer->getSwapchain().getExtent().width / (float)m_renderer->getSwapchain().getExtent().height, 0.1f, 10.0f);

			// Render Frame (Includes ImGui rendering)
			drawFrame(models, true, (void*)&ubo, sizeof(ubo)); // Uses the render API to draw a single frame
			
		}
		// Wait for the physical device (GPU) to be idle (Not working on anything) before we quit
		VK_CALL(vkDeviceWaitIdle(m_renderer->getDevice().getLogicalDevice()), "DeviceWaitIdle Failed!");
	}
}



}
