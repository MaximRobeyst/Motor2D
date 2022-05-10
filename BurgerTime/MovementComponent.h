#pragma once
#include "Component.h"
#include "Transform.h"

class dae::GameObject;
class MovementComponent : public dae::Component
{
public:
	MovementComponent(dae::GameObject* pGameObject, float speed = 10.f);
	virtual ~MovementComponent() = default;

	void Update() override;
	void Render() const override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;


protected:
	dae::TransformComponent* m_pTransformComponent;
	float m_Speed{ };
};

