#pragma once
#include "Wyvern/core.h"
#include "vertex_geometry.h"
#include "../wyvk_renderer.h"

namespace Wyvern {

class Model
{
public:
	Model(WYVKRenderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	inline VkBuffer* getVertexBuffer() { return &m_vertexBuffer.get()->getBuffer(); }
	inline VkBuffer* getIndexBuffer() { return &m_indexBuffer.get()->getBuffer(); }

	inline size_t getVertexBuffersCount() { return 1; } // were doing interleaved attributes for now so we will only need 1 buffer
	inline size_t getVertexCount() { return m_vertexCount; }
	inline size_t getIndexCount() { return m_indexCount; }
	inline size_t getVertexSize() { return m_vertexSize; }
	inline size_t getIndexSize() { return m_indexSize; }
	inline VkDeviceSize* getVertexOffsets() { return { 0 }; }

private:
	std::unique_ptr<WYVKBuffer> m_vertexBuffer;
	std::unique_ptr<WYVKBuffer> m_indexBuffer;
	size_t m_vertexCount = 0;
	size_t m_indexCount = 0;
	size_t m_vertexSize = 0;
	size_t m_indexSize = 0;

	WYVKRenderer& m_renderer;
};

}

