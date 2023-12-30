#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Entity/player.h"
#include "Wyvern/Camera/perspective_camera.h"

namespace Wyvern {

class EntityController
{
public:
	EntityController();
	~EntityController();

	void setPawn(Entity* pawn) { m_pawn = pawn;  }
	void setCamera(PerspectiveCamera* camera) { m_camera = camera; }
	void update(float deltaTime);

private:
	Entity* m_pawn;
	PerspectiveCamera* m_camera;
	glm::vec3 m_movementVector;
	double m_lastCursorX = 0.0f;
	double m_lastCursorY = 0.0f;
};

}