#include "scene.h"

namespace Wyvern {


Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::initScene()
{
	m_entityController = std::make_unique<EntityController>();
	m_player = std::make_unique<Player>();
	m_camera = std::make_unique<PerspectiveCamera>();

	m_entityController->setPawn(m_player.get());
	m_entityController->setCamera(m_camera.get());
}

void Scene::update(float deltaTime)
{
	// Updates player translation and rotation based on mouse input and key input
	m_entityController->update(deltaTime);
	// Updates external factors upon player like health, damage, animation etc...
	//m_player->update();
	// Updates the camera. Since the camera has a reference to the entity its tracking (The player) 
	// it can update its Transformation according to the players transformation
	//m_camera->update();
	
}

}
