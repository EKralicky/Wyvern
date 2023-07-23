#pragma once
#include "Wyvern/core.h"
#include "../wyvk_device.h"
#include "memory_resource.h"

namespace Wyvern {

	class WYVKImage : public WYVKMemoryResource
	{
	public:

		enum class ImageType
		{
			WYVK_NONE,
			WYVK_COLOR_IMAGE,
			WYVK_DEPTH_IMAGE,
			WYVK_STENCIL_IMAGE,
		};

		WYVKImage(WYVKDevice& device, ImageType imageType, uint32_t width, uint32_t height, VkMemoryPropertyFlags properties);
		~WYVKImage();

		VkFormat findSupportedFormat(const std::vector<VkFormat>& availableFormats, VkImageTiling& tiling, VkFormatFeatureFlags& features);
		VkImageView createImageView(VkImageAspectFlags aspectFlags);
		/*
		* Returns true if the specified image format contains a stencil component. Otherwise returns false
		*/
		bool hasStencilComponent(VkFormat format);

	private:
		static const std::vector<VkFormat> DEPTH_STENCIL_FORMATS;

		VkImage m_image;
		VkImageView m_imageView;
		VkFormat m_format;

		// Handles
		WYVKDevice& m_device;

	};

}
