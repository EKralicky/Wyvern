#pragma once
#include "Wyvern/Camera/perspective_camera.h"
#include "Wyvern/Entity/entity_controller.h"
#include "Wyvern/Renderer/API/Vulkan/Memory/resource_allocator.h"
#include "Wyvern/Renderer/API/Vulkan/Geometry/vertex_geometry.h"

namespace Wyvern {

class Scene
{
public:
	Scene(ResourceAllocator& allocator);

	/*
	* Creates entity controller, player, camera, and binds the player to
	* the camera and the camera controller
	*/
	void initScene();
	void update(float deltaTime);

	PerspectiveCamera& getMainCamera() { return *m_camera; }
	Player& getPlayer() { return *m_player; }
	std::vector<Model>& getGeometry() { return m_geometry; }

private:
	std::unique_ptr<EntityController> m_entityController;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<PerspectiveCamera> m_camera;
	std::vector<Model> m_geometry;

	// Used for buffer allocations
	ResourceAllocator& m_allocator;
};

}