#pragma once
#include "Wyvern/Components/transform.h"
#include "Wyvern/Camera/perspective_camera.h"

namespace Wyvern {

class Entity {
	
public:
	Entity(glm::vec3 position);
	~Entity();

	/*
	*/
	void update();

	/*
	* Moves the entity relative to where they are facing.
	* e.g. if the movementVector is (1, 0, -1), the entity will move 1 unit "forward" and 1 movement to the left
	* relative to where the entity is facing
	*/
	void moveRelative(glm::vec3 movementVector, float mdx, float mdy, float deltaTime);

	/*
	* When a camera is attached to an entity, the camera will "follow" the entity (for first person).
	* i.e. The transforms of the player and camera will be in sync
	*/
	void attachCamera(PerspectiveCamera& camera);

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
	PerspectiveCamera& m_camera;
	float m_baseMovementSpeed;
};

}