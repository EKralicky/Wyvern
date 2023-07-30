#pragma once
#include "Wyvern/core.h"

namespace Wyvern {

class Camera
{
public:
	/*
	* Updates the MVP matrix
	*/
	void update();


private:
	glm::mat4 m_projectionMatrix
};

}