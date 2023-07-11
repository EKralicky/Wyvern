#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "Core.h"
#include "window.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_renderer.h"
#include "Renderer/API/Vulkan/Geometry/Model.h"

namespace Wyvern {

//struct WindowPreference {
//    uint32_t length;
//    uint32_t width;
//    const char* name;
//    GLFWmonitor* monitor;
//    GLFWwindow* share;
//};

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

    void initRenderAPI();
    void mainLoop();
};

}

