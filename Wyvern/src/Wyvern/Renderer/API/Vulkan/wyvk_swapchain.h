#pragma once
#include "Wyvern/core.h"
#include "wyvk_instance.h"
#include "wyvk_surface.h"
#include "wyvk_device.h"

namespace Wyvern {

/*
* The Swapchain is essentially a queue of frames/images that are waiting to be presented on the screen
* It is mainly used to synchronize the presentation of the images with the refresh rate of the screen
* 
* he swapping is done in synchronization with the screen's refresh rate to avoid tearing. 
* "Tearing" is a visual artifact that happens when the screen displays parts of multiple frames in a single screen draw, 
* which can occur when a new frame is swapped in while the screen is in the middle of a refresh. 
* By syncing the swapping with the screen's refresh rate, only whole frames are drawn.
* 
*/
class WYVKSwapchain
{
public:
    WYVKSwapchain(WYVKInstance& instance, WYVKDevice& device, WYVKSurface& surface, Window& window);
    void destroy();

    void validateSwapchainSupport();
    void createSwapchain();

    inline VkSwapchainKHR getSwapchain() { return m_swapChain; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    // Handles
    WYVKInstance& m_instance;
    WYVKSurface& m_surface;
    WYVKDevice& m_device;  
    Window& m_window;

    VkExtent2D m_extent;
    VkFormat m_format;

    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    WYVKSurface::SurfaceSupportDetails& m_supportDetails;
    std::vector<VkImage> swapChainImages; // Swapchain images will automatically be cleaned up when the swapchain is destroyed
};

}