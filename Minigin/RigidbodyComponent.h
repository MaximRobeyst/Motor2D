#pragma once
#include "Component.h"
#include <box2d.h>
#include <functional>
#include <memory>

// https://box2d.org/documentation/index.html
namespace dae
{
	enum class PhysicsLayers
	{
		none = 0x0000,
		DefaultLayer = 0x0001,
		layer1 = 0x0002,
		layer2 = 0x0004,
		layer3 = 0x0008
	};

	class TransformComponent;
	class ColliderComponent;
	class RigidbodyComponent : public Component
	{
	public:
		RigidbodyComponent() = default;
		RigidbodyComponent(
			dae::GameObject* pGameobject, b2BodyType bodyType = b2_dynamicBody, 
			float density = 1.0f, float friction = 1.0f, bool IsSensor = false,
			PhysicsLayers layer = PhysicsLayers::DefaultLayer, uint16 mask = 0xFFFF);
		~RigidbodyComponent();

		void Start() override;
		void Update() override;
		void Render() const override;

		void RenderGUI() override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
		void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

		void OnBeginContact(dae::RigidbodyComponent* pTriggeredBody, RigidbodyComponent* pOtherBody, b2Contact* pContact);
		void OnEndContact(dae::RigidbodyComponent* pTriggeredBody, RigidbodyComponent* pOtherBody, b2Contact* pContact);

		void SetOnEnterFunction(std::function<void(RigidbodyComponent* ,RigidbodyComponent*, b2Contact*)> newOnEnterFunction);
		void SetOnExitFunction(std::function<void(RigidbodyComponent* , RigidbodyComponent*, b2Contact*)> newOnEnterFunction);

		b2Vec2 GetPosition() const;
		b2Body* GetBody() const;

		void ChangeShape(b2Shape* pShape);
		void ChangeBody(b2BodyType bodyType, float density = 1.0f, float friction = 1.0f, bool isSensor = false);

		void SetSensor(bool newValue);

	private:
		b2Body* m_pBody{};

		TransformComponent* m_pTransformComponent{};
		ColliderComponent* m_pColliderComponent{};

		std::shared_ptr<b2World> m_pWorld{};

		PhysicsLayers m_PhysicsLayer;
		uint16 m_Mask;

		std::function<void(RigidbodyComponent*,RigidbodyComponent*, b2Contact*)> m_OnEnterFunction{};
		std::function<void(RigidbodyComponent*,RigidbodyComponent*, b2Contact*)> m_OnExitFunction{};

		float m_Density{};
		float m_Friction{};
		bool m_IsSensor{};
		b2BodyType m_Bodydef{};
	};
}

