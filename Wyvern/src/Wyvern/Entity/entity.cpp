#include "entity.h"


namespace Wyvern {

	Entity::Entity(glm::vec3 position)
		:m_transform(std::make_unique<Transform>()),
		m_baseMovementSpeed(0.01f)
	{
		m_transform->setPosition(position);
	}

	void Entity::update()
	{
	}

}

