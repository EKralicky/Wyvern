#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "Core.h"

namespace Wyvern {

//struct WindowPreference {
//    uint32_t length;
//    uint32_t width;
//    const char* name;
//    GLFWmonitor* monitor;
//    GLFWwindow* share;
//};

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

class Application
{
public:

    Application();
    ~Application();
        
    void run();

private:
    GLFWwindow* m_window;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice           = VK_NULL_HANDLE;

    void initNativeWindow(int width, int height, const char* title);
    void initVulkan();
    void mainLoop();
    void cleanup();
};

}

