#pragma once
#include "Component.h"
#include <box2d.h>

namespace dae
{
	class TransformComponent;
	class RigidbodyComponent;
	class ColliderComponent :public Component
	{
	public:
		ColliderComponent() = default;
		ColliderComponent(dae::GameObject* pGameObject);
		ColliderComponent(dae::GameObject* pGameObject, float width, float height, glm::vec2 center = glm::vec2(0.f, 0.f));
		~ColliderComponent();

		void Start() override;
		void Update() override;
		void Render() const override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
		void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;


		glm::vec2 GetSize() const;
		b2PolygonShape* GetDynamicBox() const;

#ifdef _DEBUG
		void RenderGUI() override;
#endif // _DEBUG

	private:
		//TODO: Make a shape struct
		float m_Width{};
		float m_Height{};
		glm::vec2 m_Center{};

		TransformComponent* m_pTransform{};
		RigidbodyComponent* m_pRigidbody{};
		b2PolygonShape* m_DynamicBox{};
	};
}

