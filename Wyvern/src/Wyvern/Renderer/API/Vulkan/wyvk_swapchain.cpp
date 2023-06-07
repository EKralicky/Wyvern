#include "wyvk_swapchain.h"
#include <limits>

namespace Wyvern {

WYVKSwapchain::WYVKSwapchain(WYVKInstance& instance, WYVKDevice& device, WYVKSurface& surface, Window& window)
	: m_instance(instance),
	m_surface(surface),
	m_device(device),
	m_window(window),
	m_supportDetails(m_surface.getSupportDetails())
{

}

void WYVKSwapchain::destroy()
{
	for (auto imageView : m_swapChainImageViews) {
		vkDestroyImageView(m_device.getLogicalDevice(), imageView, nullptr);
	}
	vkDestroySwapchainKHR(m_device.getLogicalDevice(), m_swapChain, nullptr);
}

void WYVKSwapchain::validateSwapchainSupport()
{
	WYVERN_ASSERT(!m_supportDetails.formats.empty() && !m_supportDetails.presentModes.empty(), "Swapchain is not suitable! There are no available formats or present modes!");
}

void WYVKSwapchain::createSwapchain()
{
	// Choose support details from available support details queried in the constructor
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(m_supportDetails.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(m_supportDetails.presentModes);
	VkExtent2D extent = chooseSwapExtent(m_supportDetails.capabilities);

	m_format = surfaceFormat.format;
	m_extent = extent;

	// Minimum number of images in swapchain
	// Add one more than minimum to account for driver internal operation overhead
	uint32_t imageCount = m_supportDetails.capabilities.minImageCount + 1;

	// Make sure minimum inage count does not exceed maximum image count
	if (m_supportDetails.capabilities.maxImageCount > 0 && imageCount > m_supportDetails.capabilities.maxImageCount) {
		imageCount = m_supportDetails.capabilities.maxImageCount;
	}

	// Im going to be creating the create info struct here because there's too many variables to pass into a function and we need control
	VkSwapchainCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface.getSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	WYVERN_LOG_INFO("Using extent: {:d}, {:d}", extent.width, extent.height);
	createInfo.imageArrayLayers = 1; // Always 1
	// VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT = render directly to the swapchain. 
	// VK_IMAGE_USAGE_TRANSFER_DST_BIT = render to separate image for post and transfer to swapchain
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	WYVKDevice::QueueFamilyIndices indices = m_device.getQueueFamilyIndices();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = m_supportDetails.capabilities.currentTransform; // Image transformation like rotation or flip
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Used for blending with other windows in the window system
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE; // Ignore the color of pixels that are obscured
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CALL(vkCreateSwapchainKHR(m_device.getLogicalDevice(), &createInfo, nullptr, &m_swapChain), "Unable to create Swapchain!");

	// Retrieve handles to swapchain images
	vkGetSwapchainImagesKHR(m_device.getLogicalDevice(), m_swapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_device.getLogicalDevice(), m_swapChain, &imageCount, m_swapChainImages.data());
}

void WYVKSwapchain::createImageViews()
{
	m_swapChainImageViews.resize(m_swapChainImages.size());
	for (size_t i = 0; i < m_swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		VKInfo::createImageViewInfo(createInfo, m_swapChainImages[i], m_format);
		VK_CALL(vkCreateImageView(m_device.getLogicalDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]), "Unable to create image view! Index: " + std::to_string(i))
	}
}

/* https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain 
* Each VkSurfaceFormatKHR entry contains a format and a colorSpace member. The format member specifies the color channels and types. 
* For example, VK_FORMAT_B8G8R8A8_SRGB means that we store the B, G, R and alpha channels in that order with an 8 bit unsigned integer for a 
* total of 32 bits per pixel. The colorSpace member indicates if the SRGB color space is supported or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag. 
* Note that this flag used to be called VK_COLORSPACE_SRGB_NONLINEAR_KHR in old versions of the specification.
* We will use the SRGB color space
*/
VkSurfaceFormatKHR WYVKSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat; // Returns if we have RGBA 8 bit pixels & an SRGB color format
		}
	}
	WYVERN_LOG_WARN("Using default swapchain format!");
	return availableFormats[0]; // Default to returning first format
}

/* https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain
* The presentation mode is arguably the most important setting for the swap chain, because it represents the actual conditions for showing images to the screen. 
* There are four possible modes available in Vulkan:
*
*    VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are transferred to the screen right away, which may result in tearing.
* 
*    VK_PRESENT_MODE_FIFO_KHR: The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed 
*							   and the program inserts rendered images at the back of the queue. If the queue is full then the program has to wait. 
*							   This is most similar to vertical sync as found in modern games. The moment that the display is refreshed is known as "vertical blank".
* 
*    VK_PRESENT_MODE_FIFO_RELAXED_KHR: This mode only differs from the previous one if the application is late and the queue was empty at the last vertical blank. 
*                                      Instead of waiting for the next vertical blank, the image is transferred right away when it finally arrives. 
*									   This may result in visible tearing.
* 
*    VK_PRESENT_MODE_MAILBOX_KHR: This is another variation of the second mode. Instead of blocking the application when the queue is full, the images that are 
*                                 already queued are simply replaced with the newer ones. This mode can be used to render frames as fast as possible while still 
*								  avoiding tearing, resulting in fewer latency issues than standard vertical sync. This is commonly known as "triple buffering", although 
*								  the existence of three buffers alone does not necessarily mean that the framerate is unlocked.
*
*/
VkPresentModeKHR WYVKSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { // We want to use mailbox if its available
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

/*
* The VkExtent2D is the resolution of the swapchain images and is almost equal to 
* the resolution of the window in PIXELS
*/
VkExtent2D WYVKSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	uint32_t width = capabilities.currentExtent.width;
	if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(m_window.getNativeWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		// bound the values of width and height between the allowed minimum and maximum extents that are supported by the implementation
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

}

