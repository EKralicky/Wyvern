#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Components/transform.h"

namespace Wyvern {

class PerspectiveCamera
{
public:
	PerspectiveCamera();

	void setTarget(Entity* target) { m_target = target; }
	void update();

private:
	std::shared_ptr<Transform> m_transform;
	glm::mat4 m_projectionMatrix;
	Entity* m_target = nullptr;
	float m_targetDistance = 0.0f;
};

}
   