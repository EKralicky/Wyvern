#include "imguihandler.h"

namespace Wyvern {

	ImGuiHandler::ImGuiHandler(Window& window, WYVKRenderer& renderer)
		: m_window(window),
		m_renderer(renderer)
	{
		VkDescriptorPoolSize poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 1000;
		poolInfo.poolSizeCount = std::size(poolSizes);
		poolInfo.pPoolSizes = poolSizes;

		
		VK_CALL(vkCreateDescriptorPool(m_renderer.getDevice().getLogicalDevice(), &poolInfo, nullptr, &m_imguiPool), "Unable to create IMGUI descriptor pool!");

		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = m_renderer.getInstance().getInstance();
		initInfo.PhysicalDevice = m_renderer.getDevice().getPhysicalDevice();
		initInfo.Device = m_renderer.getDevice().getLogicalDevice();
		initInfo.Queue = m_renderer.getDevice().getGraphicsQueue();
		initInfo.DescriptorPool = m_imguiPool;
		initInfo.MinImageCount = 3;
		initInfo.ImageCount = 3;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls (we don't need gamepad for now)

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(m_window.getNativeWindow(), true);
		ImGui_ImplVulkan_Init(&initInfo, m_renderer.getRenderPass().getRenderPass());

		m_renderer.immediateSubmit([&](VkCommandBuffer cmd) {
			ImGui_ImplVulkan_CreateFontsTexture(cmd);
		});
	}

	ImGuiHandler::~ImGuiHandler()
	{
		ImGui_ImplVulkan_DestroyFontUploadObjects();
		vkDestroyDescriptorPool(m_renderer.getDevice().getLogicalDevice(), m_imguiPool, nullptr);

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplVulkan_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiHandler::newFrame()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplVulkan_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiHandler::renderFrame(WYVKCommandBuffer& cmdBuffer)
	{
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *cmdBuffer.getCommandBuffer());
	}
}
