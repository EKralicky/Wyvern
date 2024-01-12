#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/Memory/resource_allocator.h"

namespace Wyvern {

struct Vertex 
{
	glm::vec3 pos;
	glm::vec3 color;
 
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		// Index of the vertex binding in the array of bindings
		bindingDescription.binding = 0;
		// Stride. This is basically the length in bytes of the vertex attribute data so the renderer knows how much data to jump over for the next vertex
		bindingDescription.stride = sizeof(Vertex);
		// VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
		// VK_VERTEX_INPUT_RATE_INSTANCE : Move to the next data entry after each instance
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		/*
		* Attributes for POSITION
		*/
		attributeDescriptions[0].binding = 0; // Tells Vulkan from which binding the per-vertex data comes
		attributeDescriptions[0].location = 0; // References the location directive of the input in the vertex shader.
		//  float: VK_FORMAT_R32_SFLOAT
		//	vec2 : VK_FORMAT_R32G32_SFLOAT
		//	vec3 : VK_FORMAT_R32G32B32_SFLOAT
		//	vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
		//  ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
		//  uvec4: VK_FORMAT_R32G32B32A32_UINT, a 4 - component vector of 32 - bit unsigned integers
		//  double : VK_FORMAT_R64_SFLOAT
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // Type of data for the attribute (byte size)
		attributeDescriptions[0].offset = offsetof(Vertex, pos); //

		/*
		* Attributes for COLOR
		*/
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

struct Model
{
	ResourceAllocator::Buffer vertexBuffer;
	ResourceAllocator::Buffer indexBuffer;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
	size_t vertexSize = 0;
	size_t indexSize = 0;
};

}