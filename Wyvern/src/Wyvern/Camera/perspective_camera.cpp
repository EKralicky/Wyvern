#include "perspective_camera.h"
#include "Wyvern/application.h"

namespace Wyvern {

PerspectiveCamera::PerspectiveCamera()
{
	m_transform = std::make_shared<Transform>();
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)Application::get().getWindow().getWidth() / (float)Application::get().getWindow().getHeight(), 0.1f, 10.0f);
}

void PerspectiveCamera::update()
{

}

}