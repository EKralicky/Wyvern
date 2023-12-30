#pragma once
#include "Wyvern/Components/transform.h"

namespace Wyvern {

class Entity {
	
public:
	Entity(glm::vec3 position = { 0.0f, 0.0f, 0.0f });
	~Entity();

	/*
	*/
	void update();

	/*
	* Moves the entity relative to where they are facing.
	* e.g. if the movementVector is (1, 0, -1), the entity will move 1 unit "forward" and 1 movement to the left
	* relative to where the entity is facing
	*/
	void moveRelative(glm::vec3 movementVector, float deltaTime);

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