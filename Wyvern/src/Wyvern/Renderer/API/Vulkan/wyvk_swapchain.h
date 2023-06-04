//#pragma once
//#include "Wyvern/core.h"
//
//namespace Wyvern {
//
///*
//* The Swapchain is essentially a queue of frames/images that are waiting to be presented on the screen
//* It is mainly used to synchronize the presentation of the images with the refresh rate of the screen
//* 
//* he swapping is done in synchronization with the screen's refresh rate to avoid tearing. 
//* "Tearing" is a visual artifact that happens when the screen displays parts of multiple frames in a single screen draw, 
//* which can occur when a new frame is swapped in while the screen is in the middle of a refresh. 
//* By syncing the swapping with the screen's refresh rate, only whole frames are drawn.
//* 
//*/
//class WYVKSwapchain
//{
//
//public:
//    struct SwapChainSupportDetails {
//        VkSurfaceCapabilitiesKHR capabilities;
//        std::vector<VkSurfaceFormatKHR> formats;
//        std::vector<VkPresentModeKHR> presentModes;
//    };
//
//public:
//    void initialize(VkInstance instance, VkPhysicalDevice device, VkSurfaceKHR surface);
//
//private:
//    VkPhysicalDevice m_physicalDevice;  
//    SwapChainSupportDetails m_supportDetails;
//    VkSurfaceKHR m_surface;
//
//    SwapChainSupportDetails querySwapChainSupport();
//
//};
//
//}