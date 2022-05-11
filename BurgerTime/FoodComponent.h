#pragma once
#include "Component.h"
#include "SpriteRendererComponent.h"
#include <Subject.h>
#include <Collider.h>

class FoodComponent : public dae::Component
{
public:
	FoodComponent();
	FoodComponent(dae::GameObject* pGameobject);

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

#ifdef _DEBUG
	void RenderGUI() override;
#endif // _DEBUG


	void SetFalling(bool newValue);
	bool GetFalling() const;
	void SetState(int i, bool newState);
	void SetCollidingWithPlayer(bool newState, dae::GameObject* pGameobject);

	std::unique_ptr<Subject>& GetSubject();
private:
	void CollidingWithPlayer();

	bool m_Falling{ false };
	bool m_CollidingWithPlayer{ false };
	dae::GameObject* m_pPlayer{nullptr};

	glm::vec2 m_FallingSpeed = glm::vec2{ 0, 50 };

	dae::TransformComponent* m_pTransform{nullptr};
	dae::RigidbodyComponent* m_pRigidbody{ nullptr };
	dae::ColliderComponent* m_pCollider{ nullptr };
	std::unique_ptr<Subject> m_pSubject;

	static const int m_Parts{ 5 };
	bool m_PartStates[ m_Parts ];

};

