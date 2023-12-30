#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Components/transform.h"
#include "Wyvern/Entity/entity.h"

namespace Wyvern {

class PerspectiveCamera
{
public:
	PerspectiveCamera();

	void update();

	void setTarget(Entity* target) { m_target = target; }
	Entity* getTarget() { return m_target; }
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	float getSensitivity() { return m_sensitivity; }
	Transform& getTransform() { return *m_transform; }

private:
	std::unique_ptr<Transform> m_transform;
	glm::mat4 m_projectionMatrix;
	Entity* m_target = nullptr;
	
	float m_targetDistance = 0.0f;
	float m_sensitivity = 0.01f;
};

}
   