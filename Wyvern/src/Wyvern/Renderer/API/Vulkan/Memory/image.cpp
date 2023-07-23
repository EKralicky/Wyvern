#include "image.h"

namespace Wyvern {

    const std::vector<VkFormat> WYVKImage::DEPTH_STENCIL_FORMATS = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };


    WYVKImage::WYVKImage(WYVKDevice& device, ImageType imageType, uint32_t width, uint32_t height, VkMemoryPropertyFlags properties)
        : WYVKMemoryResource(device, properties),
          m_device(device)
    {
        VkImageCreateInfo imageInfo{};

        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        switch (imageType)
        {
        case ImageType::WYVK_DEPTH_IMAGE:
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.format = findSupportedFormat(DEPTH_STENCIL_FORMATS, imageInfo.tiling, imageInfo.usage);
            break;

        case ImageType::WYVK_STENCIL_IMAGE:
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.format = VK_FORMAT_S8_UINT;
            break;

        case ImageType::WYVK_COLOR_IMAGE:
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            break;

        case ImageType::WYVK_NONE:
        default:
            WYVERN_LOG_ERROR("Unsupported image type!");
            throw std::runtime_error("Unsupported image type!");
        }

        // Set format for future use
        m_format = imageInfo.format;

        // Create Vulkan image
        VK_CALL(vkCreateImage(m_device.getLogicalDevice(), &imageInfo, nullptr, &m_image), "Failed to create image!");

        // Get memory creation requirements
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_device.getLogicalDevice(), m_image, &memRequirements);

        // Allocates memory on m_deviceMemory. This allocation is done in the base class. 
        // see WYVKMemoryResource for implementation details
        allocateMemory(memRequirements);

        // Bind the device memory to the image
        vkBindImageMemory(m_device.getLogicalDevice(), m_image, m_deviceMemory, 0);
    }

    WYVKImage::~WYVKImage()
    {
        vkDestroyImage(m_device.getLogicalDevice(), m_image, nullptr);
    }

    VkFormat WYVKImage::findSupportedFormat(const std::vector<VkFormat>& availableFormats, VkImageTiling& tiling, VkFormatFeatureFlags& features)
    {
        for (VkFormat format : availableFormats) {
            // First retrieve format properties which includes linear and optimal tiling features
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_device.getPhysicalDevice(), format, &props);

            // If tiling parameter is LINEAR, check if the features we want have linear tiling features
            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            // If tiling parameter is OPTIMAL, check if the features we want have optimal tiling features
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        WYVERN_LOG_ERROR("Unable to find supported image format!");
        WYVERN_ASSERT(false, "Unable to find supported image format!");
    }

    VkImageView WYVKImage::createImageView(VkImageAspectFlags aspectFlags)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        VK_CALL(vkCreateImageView(m_device.getLogicalDevice(), &viewInfo, nullptr, &imageView), "Failed to create image view!");

        return imageView;
    }

    
    bool WYVKImage::hasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }



}


