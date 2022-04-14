#pragma once
#include "Component.h"
#include "SpriteRendererComponent.h"
#include <Subject.h>

class FoodComponent : public dae::Component
{
public:
	FoodComponent(dae::GameObject* pGameobject);

	void Update() override;

#ifdef _DEBUG
	void RenderGUI() override;
#endif // _DEBUG


	void SetFalling(bool newValue);
	bool GetFalling() const;

	std::unique_ptr<Subject>& GetSubject();
private:
	bool m_Falling{ false };
	glm::vec2 m_FallingSpeed = glm::vec2{ 0, 50 };

	dae::TransformComponent* m_pTransform{nullptr};
	dae::RigidbodyComponent* m_pRigidbody{ nullptr };
	std::unique_ptr<Subject> m_pSubject;

};

