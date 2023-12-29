#pragma once
#include "Wyvern/Camera/perspective_camera.h"
#include "Wyvern/Entity/entity_controller.h"

namespace Wyvern {

class Scene
{
public:
	Scene();
	~Scene();

	void initScene();
	void update(float deltaTime);

	PerspectiveCamera& getMainCamera() { return *m_camera; }
	Player& getPlayer() { return *m_player; }

private:
	std::unique_ptr<EntityController> m_entityController;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<PerspectiveCamera> m_camera;
};

}