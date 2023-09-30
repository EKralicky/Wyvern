#pragma once
#include "Camera/perspective_camera.h"
#include "Entity/player.h"

/*
The scene will include:

Terrain meshes (chunks and stuff)
Entity meshes
Player
Camera
Other scene properties

My idea is to pass the scene into the render frame function and it takes all of the meshes and renders them. 


*/


namespace Wyvern {

class Scene
{
public:
	Scene();
	~Scene();
	void update();

	inline const PerspectiveCamera& getCamera() const { return m_camera; }
	inline Player& getPlayer() { return m_player; }

private:
	PerspectiveCamera m_camera;
	Player m_player;

};

}