#pragma once
#include "Wyvern/Components/transform.h"

namespace Wyvern {

class Entity {
	
public:
	Entity(glm::vec3 position = { 0.0f, 0.0f, 0.0f });
	/*
	*/
	void update();

	/*
	* Sets the entities base movement speed.
	* The base movement speed is multiplied by normalized facing vectors in the transform to move the player a scaled distance
	*/
	void setBaseMovementSpeed(float speed) { m_baseMovementSpeed = speed; }

	/*
	* Returns the entities transform
	*/
	Transform& getTransform() { return *m_transform; }

private:
	std::unique_ptr<Transform> m_transform;
	float m_baseMovementSpeed;
};

}