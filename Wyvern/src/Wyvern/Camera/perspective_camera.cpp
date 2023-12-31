#include "perspective_camera.h"
#include "Wyvern/application.h"

namespace Wyvern {

PerspectiveCamera::PerspectiveCamera()
	:m_transform(std::make_unique<Transform>()),
	m_projectionMatrix(glm::perspective(glm::radians(45.0f), (float)Application::get()->getWindow().getWidth() / (float)Application::get()->getWindow().getHeight(), 0.1f, 1000.0f))
{	
}

void PerspectiveCamera::update()
{

}

glm::mat4 PerspectiveCamera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

/*
* There's probably a better way to do this but this is the simplest I found
*/
void PerspectiveCamera::updateTransformFromTarget()
{
	m_transform->setValues(
		m_target->getTransform().getPosition(),
		m_target->getTransform().getOrientation(),
		m_target->getTransform().getFront()
	);
}

glm::mat4 PerspectiveCamera::getViewMatrix()
{
	return m_transform->getViewMatrix();
}

}