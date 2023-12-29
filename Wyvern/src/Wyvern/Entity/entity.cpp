#include "entity.h"


namespace Wyvern {

	Entity::Entity(glm::vec3 position)
		:m_camera()
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::update()
	{
	}

	void Entity::move(glm::vec3 movementVector, float mdx, float mdy, float deltaTime)
	{
	}

	void Entity::attachCamera(PerspectiveCamera& camera)
	{
	}

}

