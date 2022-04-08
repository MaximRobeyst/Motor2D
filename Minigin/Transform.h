#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		struct Transform
		{
			glm::vec3 position{};
			glm::vec4 rotation{};
			glm::vec3 scale{};
		};

		TransformComponent(GameObject* pGameobject, glm::vec3 position = glm::vec3{}, glm::vec3 scale = glm::vec3{1,1,1});

		// Position
		const glm::vec3& GetPosition() const { return m_Transform.position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		void Move(const glm::vec3& moveVector);

		//Scale
		const glm::vec3& GetScale() const { return m_Transform.scale; }
		void SetScale(float x, float y, float z);
		void SetScale(const glm::vec3& scale);

		void Update() override;
		void Render() const override;

		Transform& GetTransform();

	private:
		Transform m_Transform;
		TransformComponent* m_pParentComponent;
	};
}
