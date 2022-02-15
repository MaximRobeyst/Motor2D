#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pGameobject, glm::vec3 position = glm::vec3{});

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

		void Update() override;
		void Render() const override;
	private:
		glm::vec3 m_Position{};
	};
}
