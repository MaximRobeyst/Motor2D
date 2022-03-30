#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class ColliderComponent final :public Component
	{
	public:
		explicit ColliderComponent(dae::GameObject* pGameObject);
		explicit ColliderComponent(dae::GameObject* pGameObject, float width, float height);
		~ColliderComponent() = default;
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		void Update() override;
		void Render() const override;

	private:
		//TODO: Make a shape struct
		float m_Width{};
		float m_Height{};
		glm::vec2 m_Center{};

		TransformComponent* m_pTransform;
	};
}

