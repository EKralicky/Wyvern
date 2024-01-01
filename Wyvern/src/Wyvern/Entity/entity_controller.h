#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Entity/player.h"
#include "Wyvern/Camera/perspective_camera.h"

namespace Wyvern {

class EntityController
{
public:
	EntityController(Entity* pawn);
	EntityController();

	void update(float deltaTime);
	void setPawn(Entity* pawn) { m_pawn = pawn; }
	float getSensitivity() { return m_sensitivity; }

private:
	Entity* m_pawn;
	glm::vec3 m_movementVector;

	double m_lastCursorX = 0.0f;
	double m_lastCursorY = 0.0f;
	bool m_firstMouse = true;
	/*
	* The sensitivity should be an attribute of the controller and not the camera.
	* This makes sense because sensitivity is not inherently an attribute of a camera. 
	* A camera is just something that lets us view the world, while the sensitivity is part of how the user controls it. Therefore, 
	* it should be in a controller class. 
	*
	* Think of it like a person holding a camera. Does the camera control how fast the person moves it?
	*/
	float m_sensitivity = 0.1f;
};

}