#pragma once
#include "Wyvern/core.h"

namespace Wyvern {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera();


		inline void setPosition(glm::vec3 pos) { m_position = pos; };
		inline const glm::vec3& getPosition() const { return m_position; }


		void updateMatrices();

	private:
		glm::vec3 m_position;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewProjectionMatrix;
	};

}
