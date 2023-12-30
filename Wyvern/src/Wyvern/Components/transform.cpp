#include "transform.h"

namespace Wyvern {

Transform::Transform(glm::vec3 position)
	:m_position({ 0.0f, 0.0f, 0.0f }),
	m_orientation({ 1.0f, 0.0f, 0.0f, 0.0f }),
	m_front({1.0f, 0.0f, 0.0f})
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
	glm::quat quatPitch = glm::angleAxis(m_pitchAngle, glm::vec3(1, 0, 0));
	glm::quat quatYaw = glm::angleAxis(m_yawAngle, glm::vec3(0, 1, 0));

	m_orientation = glm::normalize(quatYaw * quatPitch);
	m_front = { m_orientation.x, m_orientation.y, m_orientation.z };
}

void Transform::updatePosition(glm::vec3 relativeVelocity)
{

}

void Transform::setPosition(glm::vec3 position)
{
	m_position = position;
}

}


