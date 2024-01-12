#pragma once
#include "Wyvern/core.h"
#include "vertex_geometry.h"
#include "vma/vk_mem_alloc.h"
#include "../Memory/buffer.h"

namespace Wyvern {

class Model
{
public:
	Model(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	void setVertexBuffer(VkBuffer* buffer) { m_vertexBuffer.reset(buffer); }
	void setIndexBuffer(VkBuffer* buffer) { m_indexBuffer.reset(buffer); }
	VkBuffer* getVertexBuffer() { return m_vertexBuffer.get(); }
	VkBuffer* getIndexBuffer() { return m_indexBuffer.get(); }

	size_t getVertexBuffersCount() { return 1; } // were doing interleaved attributes for now so we will only need 1 buffer
	size_t getVertexCount() { return m_vertexCount; }
	size_t getIndexCount() { return m_indexCount; }
	size_t getVertexSize() { return m_vertexSize; }
	size_t getIndexSize() { return m_indexSize; }
	VkDeviceSize* getVertexOffsets() { return { 0 }; }

private:
	std::unique_ptr<VkBuffer> m_vertexBuffer;
	std::unique_ptr<VkBuffer> m_indexBuffer;
	size_t m_vertexCount = 0;
	size_t m_indexCount = 0;
	size_t m_vertexSize = 0;
	size_t m_indexSize = 0;
};

}

