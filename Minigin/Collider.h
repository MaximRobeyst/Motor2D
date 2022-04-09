#pragma once
#include "Component.h"
#include <box2d.h>

namespace dae
{
	class TransformComponent;
	class ColliderComponent :public Component
	{
	public:
		ColliderComponent(dae::GameObject* pGameObject);
		ColliderComponent(dae::GameObject* pGameObject, float width, float height, glm::vec2 center = glm::vec2(0.f, 0.f));
		~ColliderComponent();

		void Update() override;
		void Render() const override;

		glm::vec2 GetSize() const;

#ifdef _DEBUG
		void RenderGUI() override;
#endif // _DEBUG

		b2PolygonShape* GetDynamicBox() const;


	private:
		//TODO: Make a shape struct
		float m_Width{};
		float m_Height{};
		glm::vec2 m_Center{};

		TransformComponent* m_pTransform;
		b2PolygonShape* m_DynamicBox;
	};
}

