#include "transform.h"

namespace Wyvern {

Transform::Transform(glm::vec3 position)
	:m_position(position),
	m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	m_front({0.0f, 0.0f, 1.0f})
{
}

glm::mat4 Transform::getViewMatrix()
{
	glm::quat inverse = glm::conjugate(m_orientation);
	glm::mat4 rotate = glm::mat4_cast(inverse);
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -m_position);
	 
	return (rotate * translate);
}

void Transform::updateOrientation(float deltaPitch, float deltaYaw, float scale)
{
	deltaPitch *= scale;
	deltaYaw *= scale;

	m_pitchAngle += deltaPitch;
	m_yawAngle += deltaYaw;

	if (m_lockPitch) {
		if (m_pitchAngle > 90.0f) {
			m_pitchAngle = 90.0f;
		}
		else if (m_pitchAngle < -90.0f) {
			m_pitchAngle = -90.0f;
		}
	}

	if (m_yawAngle > 360.0f) {
		m_yawAngle -= 360.0f;
	}
	else if (m_yawAngle < 0.0f) {
		m_yawAngle += 360.0f;
	}
	
	updateQuatOrientation();
}

void Transform::updateQuatOrientation()
{
	glm::quat quatPitch = glm::angleAxis(glm::radians(m_pitchAngle), glm::vec3(1, 0, 0));
	glm::quat quatYaw = glm::angleAxis(glm::radians(m_yawAngle), glm::vec3(0, 1, 0));

	m_orientation = glm::normalize(quatYaw * quatPitch);
	m_front = glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::updatePosition(glm::vec3& relativeVelocity, float scale)
{
	glm::vec3 left = glm::cross(m_front, m_worldUp);
	m_position += relativeVelocity.x * left * scale;
	m_position += relativeVelocity.y * m_worldUp * scale;
	m_position += relativeVelocity.z * m_front * scale;
}

void Transform::setPosition(glm::vec3 position)
{
	m_position = position;
}

}


