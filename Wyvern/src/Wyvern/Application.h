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
#include "Wyvern/Events/event.h"

#include "Entity/player.h"
#include "scene.h"


namespace Wyvern {

#define BIND_INTERNAL_FUNC(func) std::bind(&func, this)                                     // Can be used to bind a function within the class this is used in
#define BIND_EXTERNAL_FUNC(func, instance) std::bind(&func, std::ref(instance))             // Used to bind a function outside the class this is used in. Uses std::ref so we get the currently used instance not a copy

#define BIND_INTERNAL_EVENT(func) std::bind(&func, this, std::placeholders::_1)                                         // Event functions will have a placeholder for the `Event` type parameter. The onEvent function must be a member of the class this is used in
#define BIND_EXTERNAL_EVENT(func, instance) std::bind(&func, std::ref(instance), std::placeholders::_1)                 // Event functions will have a placeholder for the `Event` type parameter. The onEvent function must be a member of the class this is used in

static const uint32_t WINDOW_WIDTH = 1920;
static const uint32_t WINDOW_HEIGHT = 1080;

class Application
{
public:
    Application();
    ~Application();
        
    void run();
    void onEvent(Event& event);
    bool onWindowClose(WindowCloseEvent& event);
    void drawFrame(std::vector<Model>& models, bool drawIndexed, void* uniformData, size_t uniformSize);
    Window& getWindow() { return *m_window; }

    static Application* get() { 
        if (!s_Instance) {
            s_Instance = new Application();
        }
        return s_Instance;
    };

private:
    uint32_t m_currentFrame = 0;

    std::unique_ptr<Logger> m_logger;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<WYVKRenderer> m_renderer;
    std::unique_ptr<ImGuiHandler> m_imGuiHandler;
    std::unique_ptr<Scene> m_scene;

    // Is the application running? Will be set to false on windowCloseEvent
    bool m_running = true;
    inline static Application* s_Instance;

    //long long m_frameTime;
    void mainLoop();
};

    Application* createApplication();

}

