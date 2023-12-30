#include "entity.h"


namespace Wyvern {

	Entity::Entity(glm::vec3 position)
		:m_transform(std::make_unique<Transform>()),
		m_baseMovementSpeed(1.0f)
	{
		m_transform->setPosition(position);
	}

	Entity::~Entity()
	{
	}

	void Entity::update()
	{
	}

	void Entity::moveRelative(glm::vec3 movementVector, float deltaTime)
	{
	}

}

