#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "Core.h"
#include "window.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_renderer.h"
#include "Renderer/API/Vulkan/Geometry/Model.h"
#include "Wyvern/GUI/imguihandler.h"

#include "Entity/player.h"
#include "Camera/camera.h"

namespace Wyvern {

#define BIND_VOID_FUNC(func, instance) std::bind(&func, std::ref(instance))                         // No parameters. This will be used for adding keybinds in the InputManager
#define BIND_EVENT_FUNC(func, instance) std::bind(&func, std::ref(instance), std::placeholders::_1) // Event functions will have a placeholder for the `Event` type parameter

static const uint32_t WINDOW_WIDTH = 800;
static const uint32_t WINDOW_HEIGHT = 600;

class Application
{
public:
    Application();
    ~Application();
        
    void run();
    void drawFrame(std::vector<Model>& models, bool drawIndexed, void* uniformData, size_t uniformSize);

private:
    uint32_t m_currentFrame = 0;

    std::unique_ptr<Logger> m_logger;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<WYVKRenderer> m_renderer;
    std::unique_ptr<ImGuiHandler> m_imGuiHandler;

    // Game stuff
    Player m_player;
    Camera m_camera;

    void mainLoop();
};

}

