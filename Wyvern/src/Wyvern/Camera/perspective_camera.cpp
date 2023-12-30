#include "perspective_camera.h"
#include "Wyvern/application.h"

namespace Wyvern {

PerspectiveCamera::PerspectiveCamera()
	:m_transform(std::make_unique<Transform>()),
	m_projectionMatrix(glm::perspective(glm::radians(45.0f), (float)Application::get()->getWindow().getWidth() / (float)Application::get()->getWindow().getHeight(), 0.1f, 10.0f))
{	
}

void PerspectiveCamera::update()
{

}

glm::mat4 PerspectiveCamera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

glm::mat4 PerspectiveCamera::getViewMatrix()
{
	return m_transform->getViewMatrix();
}

}