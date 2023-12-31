#pragma once
#include "Wyvern/core.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Wyvern {

class Transform 
{
public:
	Transform(glm::vec3 position = { 0.0f, 0.0f, 0.0f });

	/*
	* Returns a view matrix based on the rotation and position of the transform
	*/
	glm::mat4 getViewMatrix();

	/*
	* Takes a delta pitch and yaw, and updates the orientation quaternion based on how much
	* The pitch and yaw moved.
	*/
	void updateOrientation(float deltaPitch, float deltaYaw, float scale);
	/*
	* Updates the world position given a relative velocity vector.
	* This vector is relative to the transforms orientation, so for example a vector of
	* (1.0f, 0.0f, -1.0f) will move the object forward and to the left
	*/
	void updatePosition(glm::vec3& relativeVelocity);

	void setPosition(glm::vec3& position);

	void setValues(glm::vec3 position, glm::quat orientation, glm::vec3 front)
	{
		m_position = position;
		m_orientation = orientation;
		m_front = front;
	}

	glm::vec3 getPosition() { return m_position; }
	glm::quat getOrientation() { return m_orientation; }
	glm::vec3 getFront() { return m_front; }

private:
	glm::quat m_orientation;
	glm::vec3 m_front;
	glm::vec3 m_position;
	glm::vec3 m_worldUp = { 0.0f, 1.0f, 0.0f };

	float m_pitchAngle = 0.0f; // vertical (up and down)
	float m_yawAngle = 0.0f; // horizontal (left and right)
};

}
