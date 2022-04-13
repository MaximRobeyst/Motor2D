#pragma once
#include "Component.h"

namespace dae
{
	class RigidbodyComponent;
	class TransformComponent final : public Component
	{
	public:
		struct Transform
		{
			glm::vec3 worldPosition{};
			glm::vec3 localPosition{};
			float rotation{};
			glm::vec2 scale{};
		};

		TransformComponent(GameObject* pGameobject, glm::vec3 position = glm::vec3{}, glm::vec2 scale = glm::vec2{ 1 });

		// Position
		const glm::vec3& GetPosition() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec3& position);

		void Move(const glm::vec3& moveVector);

		//Scale
		const glm::vec2& GetScale() const { return m_Transform.scale; }
		void SetScale(float x, float y);
		void SetScale(const glm::vec2& scale);

		void SetDirty();
		bool IsDirty() const;

		void Start() override;
		void Update() override;
		void Render() const override;

		Transform& GetTransform();
		const Transform& GetTransformConst() const;

	private:
		bool m_Dirty{true};

		Transform m_Transform;
		TransformComponent* m_pParentComponent;
		RigidbodyComponent* m_pRigidbodyComponent;
		

	};
}
