#pragma once
#include "Wyvern/core.h"

namespace Wyvern {

class Transform 
{
public:
	Transform(glm::vec3 position);
	~Transform();

	glm::mat4 getViewMatrix();
	void updateRotation();
	void setPosition();

private:
	glm::vec3 m_position;
	glm::vec3 m_frontVector;
	glm::vec3 m_rightVector = glm::vec3(1.0f, 0.0f, 0.0f); // points at positive x
	glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
};

}
