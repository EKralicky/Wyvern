#include "scene.h"

namespace Wyvern {


Scene::Scene(ResourceAllocator& allocator)
	: m_player(std::make_unique<Player>(glm::vec3(0.0f, 0.0f, 0.0f))),
	m_entityController(std::make_unique<EntityController>()),
	m_camera(std::make_unique<PerspectiveCamera>()),
	m_allocator(allocator)
{
}

void Scene::initScene()
{
	m_entityController->setPawn(m_player.get());
	m_camera->setTarget(m_player.get());

	const std::vector<Vertex> vertices = {
		// Front (clockwise)
		{{0.0f, 0.0f, 0.0f}, {0.718f, 1.0f, 0.396f}},
		{{1.0f, 0.0f, 0.0f}, {0.651f, 0.0f, 1.0f}},
		{{1.0f, 0.0f, 1.0f}, {0.718f, 1.0f, 0.396f}},
		{{0.0f, 0.0f, 1.0f}, {0.651f, 0.0f, 1.0f}},

		// Back (cc)
		{{0.0f, 1.0f, 0.0f}, {0.718f, 1.0f, 0.396f}},
		{{1.0f, 1.0f, 0.0f}, {0.651f, 0.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {0.718f, 1.0f, 0.396f}},
		{{0.0f, 1.0f, 1.0f}, {0.651f, 0.0f, 1.0f}},
	};
	const std::vector<uint16_t> indices = {
		// Front C
		0, 5, 1,
		0, 4, 5,
		// Back CC
		3, 2, 6,
		3, 6, 7,
		// Top C
		4, 6, 5,
		4, 7, 6,
		// Bottom
		0, 1, 2,
		0, 2, 3,
		// Right
		1, 6, 2,
		1, 5, 6,
		// Left
		0, 3, 7,
		0, 7, 4
	};

	Model terrain;
	terrain.vertexCount = vertices.size();
	terrain.indexCount = indices.size();
	terrain.vertexSize = sizeof(vertices[0]);
	terrain.indexSize = sizeof(indices[0]);
	terrain.vertexBuffer = m_allocator.createBuffer((void*)vertices.data(),
		terrain.vertexCount * terrain.vertexSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU,
		false);
	terrain.indexBuffer = m_allocator.createBuffer((void*)indices.data(),
		terrain.indexCount * terrain.indexSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU,
		false);

	m_geometry.push_back(terrain);
}

void Scene::update(float deltaTime)
{
	// Updates player translation and rotation based on mouse input and key input
	m_entityController->update(deltaTime);
	m_camera->updateTransformFromTarget(); 
	// Updates external factors upon player like health, damage, animation etc...
	//m_player->update();
	// Updates the camera. Since the camera has a reference to the entity its tracking (The player) 
	// it can update its Transformation according to the players transformation
	//m_camera->update();
}
 
}
