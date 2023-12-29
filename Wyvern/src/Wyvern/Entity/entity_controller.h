#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Entity/player.h"

namespace Wyvern {

class EntityController
{
public:
	EntityController();
	~EntityController();

	void setPawn(Entity* pawn);
	void update(float deltaTime);

private:
	Entity* m_pawn;
	glm::vec3 m_movementVector;
	double* m_deltaCursorX;
	double* m_deltaCursorY;
};

}