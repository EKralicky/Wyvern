 #include <vector>
#include <functional>

#include "Application.h"
#include "entry_point.h"

#include "Renderer/API/Vulkan/Geometry/vertex_geometry.h"
#include "Renderer/API/Vulkan/Memory/buffer.h"
#include "Renderer/API/Vulkan/Memory/resource_allocator.h"
#include "Wyvern/Input/input.h"
#include "Scene.h"

namespace Wyvern {

Application::Application()
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
	Input::initInput(m_window.get()->getNativeWindow());

	/*InputManager::getInstance().bindKey(
		KeyRequirement(WYVERN_KEY_SPACE, WYVERN_MOD_NONE),
		InputAction("Jump", InputType::CONTINUOUS, BIND_EXTERNAL_FUNC(Player::jump, m_player)));*/

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
	/*dispatcher.dispatch<MouseMovedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onMouseMoved,	InputManager::getInstance()));
	dispatcher.dispatch<MouseScrolledEvent>	(BIND_EXTERNAL_EVENT(InputManager::onMouseScrolled,	InputManager::getInstance()));
	dispatcher.dispatch<KeyPressedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onKeyPressed,	InputManager::getInstance()));
	dispatcher.dispatch<KeyReleasedEvent>	(BIND_EXTERNAL_EVENT(InputManager::onKeyReleased,	InputManager::getInstance()));*/
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
		

		m_renderer->bindVertexBuffers(m_currentFrame, 1, &model.vertexBuffer.buffer);
		m_renderer->bindIndexBuffer(m_currentFrame, model.indexBuffer.buffer, VK_INDEX_TYPE_UINT16);
		m_renderer->bindDescriptorSets(m_currentFrame);

		if (drawIndexed) {
			m_renderer->drawIndexed(m_currentFrame, model.indexCount, 1, 0, 0, 0);
		}
		else {
			m_renderer->draw(m_currentFrame, model.vertexCount, 1, 0, 0);
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

		// Create Scene
		m_scene = std::make_unique<Scene>(m_renderer->getResourceAllocator());
		m_scene->initScene();

		while (!m_window->shouldClose()) {
			m_window->pollEvents();
			m_window->updateDeltaTime();
			m_scene->update(m_window->deltaTime());

			// == ImGui ==
			m_imGuiHandler->newFrame();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Pitch: %.1f", *m_scene->getPlayer().getTransform().getPitchAngle());
			ImGui::Text("Yaw: %.1f", *m_scene->getPlayer().getTransform().getYawAngle());
			glm::vec3& front = m_scene->getPlayer().getTransform().getFront();
			ImGui::Text("Front: [%.1f,%.1f,%.1f]", front.x, front.y, front.z);

			WYVKRenderer::CameraMVPBuffer ubo{};
			static auto startTime = std::chrono::high_resolution_clock::now();
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			ubo.model = glm::mat4(1);
			ubo.view = m_scene->getMainCamera().getViewMatrix();
			ubo.proj = m_scene->getMainCamera().getProjectionMatrix();
			
			drawFrame(m_scene->getGeometry(), true, (void*)&ubo, sizeof(ubo)); // Uses the render API to draw a single frame
		}
		// Wait for the physical device (GPU) to be idle (Not working on anything) before we quit
		VK_CALL(vkDeviceWaitIdle(m_renderer->getDevice().getLogicalDevice()), "DeviceWaitIdle Failed!");
	}
}


Application* createApplication()
{
	return Application::get();
}

}


