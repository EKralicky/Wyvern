#include "entity_controller.h"
#include "Wyvern/Input/input.h"
#include "Wyvern/application.h"

namespace Wyvern {

void EntityController::setPawn(Entity* pawn)
{
	m_pawn = pawn;
}

EntityController::EntityController()
{
	*m_deltaCursorX = 0.0f;
	*m_deltaCursorY = 0.0f;
}

EntityController::~EntityController()
{

}

void EntityController::update(float deltaTime)
{
	m_movementVector = glm::vec3(0.0f, 0.0f, 0.0f);
	double* cursorX;
	double* cursorY;

	if (Input::isKeyPressed(WYVERN_KEY_W)) {
		m_movementVector.x += 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_S)) {
		m_movementVector.x -= 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_A)) {
		m_movementVector.z += 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_D)) {
		m_movementVector.z -= 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_SPACE)) {
		m_movementVector.y += 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_LEFT_CONTROL)) {
		m_movementVector.y -= 1.0f;
	}
	glfwGetCursorPos(Application::get().getWindow().getNativeWindow(), cursorX, cursorY);
	*m_deltaCursorX = *cursorX - *m_deltaCursorX;
	*m_deltaCursorY = *cursorY - *m_deltaCursorY;

	// Update entity transform vectors based on mouse position before moving relative
	m_pawn->getTransform().updateRotation();

	// Move the player relative to their rotation
	m_pawn->moveRelative(m_movementVector);

}

}


