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
		void createFrameDataPlot(float frametime);

	private:
		void setStyle();
		void setDocking();

		// utility structure for realtime plot
		struct ScrollingBuffer {
			int MaxSize;
			int Offset;
			ImVector<ImVec2> Data;
			ScrollingBuffer(int max_size = 2000) {
				MaxSize = max_size;
				Offset = 0;
				Data.reserve(MaxSize);
			}
			void AddPoint(float x, float y) {
				if (Data.size() < MaxSize)
					Data.push_back(ImVec2(x, y));
				else {
					Data[Offset] = ImVec2(x, y);
					Offset = (Offset + 1) % MaxSize;
				}
			}
			void Erase() {
				if (Data.size() > 0) {
					Data.shrink(0);
					Offset = 0;
				}
			}
		};


		// utility structure for realtime plot
		struct RollingBuffer {
			float Span;
			ImVector<ImVec2> Data;
			RollingBuffer() {
				Span = 10.0f;
				Data.reserve(2000);
			}
			void AddPoint(float x, float y) {
				float xmod = fmodf(x, Span);
				if (!Data.empty() && xmod < Data.back().x)
					Data.shrink(0);
				Data.push_back(ImVec2(xmod, y));
			}
		};

		VkDescriptorPool m_imguiPool;

		// Handles
		Window& m_window;
		WYVKRenderer& m_renderer;
	};



}


