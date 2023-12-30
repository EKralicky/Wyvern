#pragma once
#include "Wyvern/core.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Wyvern {

class Transform 
{
public:
	Transform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 getViewMatrix();
	void updateRotation(float dx, float dy, float scale);
	void setPosition(glm::vec3 position);

private:
	glm::quat m_qFront;
	glm::vec3 m_position;
	glm::vec3 m_worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_pitchAngle = 0.0f; // vertical (up and down)
	float m_yawAngle = 0.0f; // horizontal (left and right)
};

}
