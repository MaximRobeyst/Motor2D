#pragma once
#include "Component.h"

namespace dae
{
	class Transform final : public Component
	{
	public:
		Transform(GameObject* pGameobject);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position;
	};
}
