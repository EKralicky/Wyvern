#include "entity_controller.h"
#include "Wyvern/Input/input.h"
#include "Wyvern/application.h"

namespace Wyvern {

EntityController::EntityController()
{
}

EntityController::~EntityController()
{

}

void EntityController::update(float deltaTime)
{
	glm::vec3 movementVector = { 0.0f, 0.0f, 0.0f };
	double cursorX;
	double cursorY;

	if (Input::isKeyPressed(WYVERN_KEY_W)) {
		movementVector.z += 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_S)) {
		movementVector.z -= 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_A)) {
		movementVector.x += 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_D)) {
		movementVector.x -= 1.0f;
	}
	if (Input::isKeyPressed(WYVERN_KEY_SPACE)) {
		movementVector.y += 1.0f; // Using negative her because vulkan uses a coordinate system where the y coordinate goes down
	}
	if (Input::isKeyPressed(WYVERN_KEY_LEFT_CONTROL)) {
		movementVector.y -= 1.0f;
	}
	//WYVERN_LOG_INFO(movementVector.x);
	glfwGetCursorPos(Application::get()->getWindow().getNativeWindow(), &cursorX, &cursorY);

	if (m_firstMouse) {
		m_lastCursorX = cursorX;
		m_lastCursorY = cursorY;
		m_firstMouse = false;
	}

	float deltaCursorX = static_cast<float>(cursorX - m_lastCursorX);
	float deltaCursorY = static_cast<float>(cursorY - m_lastCursorY);
	m_lastCursorX = cursorX;
	m_lastCursorY = cursorY;

	// Update entity transform vectors based on mouse position before moving relative
	m_pawn->getTransform().updateOrientation(deltaCursorY, deltaCursorX, m_sensitivity);
	m_pawn->getTransform().updatePosition(movementVector);
}

}


