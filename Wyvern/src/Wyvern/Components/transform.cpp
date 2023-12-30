#include "transform.h"

namespace Wyvern {

Transform::Transform(glm::vec3 position)
	:m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_qFront(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
{
}

glm::mat4 Transform::getViewMatrix()
{
	glm::mat4 rotate = glm::mat4_cast(m_qFront);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -m_position);

	return (rotate * translate);
}

void Transform::updateRotation(float dx, float dy, float scale)
{
	WYVERN_LOG_INFO("dydy", m_pitchAngle);
	dx *= scale;
	dy *= scale;

	m_pitchAngle += dy;
	m_yawAngle += dx;
	glm::quat quatPitch = glm::angleAxis(m_pitchAngle, glm::vec3(1, 0, 0));
	glm::quat quatYaw = glm::angleAxis(m_yawAngle, glm::vec3(0, 1, 0));

	m_qFront = glm::normalize(quatYaw * quatPitch);
}

void Transform::setPosition(glm::vec3 position)
{
	m_position = position;
}

}


