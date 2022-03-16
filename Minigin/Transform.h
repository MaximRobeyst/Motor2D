#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pGameobject, glm::vec3 position = glm::vec3{}, glm::vec3 scale = glm::vec3{1,1,1});

		// Position
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		//Scale
		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);
		void SetScale(const glm::vec3& scale);

		void Update() override;
		void Render() const override;
	private:
		glm::vec3 m_Position{};
		glm::vec3 m_Scale{};
		TransformComponent* m_pParentComponent;
	};
}
