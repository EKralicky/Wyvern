#include "Model.h"

namespace Wyvern {

Model::Model(WYVKRenderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
	: m_vertexCount(vertices.size()),
	m_indexCount(indices.size()),
	m_vertexSize(sizeof(vertices[0]) * m_vertexCount),
	m_indexSize(sizeof(indices[0]) * m_indexCount),
	m_renderer(renderer)
{
	m_vertexBuffer.reset(m_renderer.createVertexBuffer((void*)vertices.data(), m_vertexSize));
	m_indexBuffer.reset(m_renderer.createIndexBuffer((void*)indices.data(), m_indexSize));
}  

}
