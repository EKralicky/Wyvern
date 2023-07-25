#pragma once
#include "Wyvern/core.h"
#include "Wyvern/window.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_renderer.h"


namespace Wyvern {

	class ImGuiHandler
	{
	public:
		ImGuiHandler(Window& window, WYVKRenderer& renderer);
		~ImGuiHandler();

		void newFrame();
		void renderFrame(WYVKCommandBuffer& cmdBuffer);

	private:
		VkDescriptorPool m_imguiPool;

		// Handles
		Window& m_window;
		WYVKRenderer& m_renderer;
	};



}


