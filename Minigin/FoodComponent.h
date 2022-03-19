#pragma once
#include "Component.h"
#include "SpriteRendererComponent.h"
#include "Subject.h"

class FoodComponent : public dae::Component
{
public:
	FoodComponent(dae::GameObject* pGameobject);

	void Update() override;

	void SetFalling(bool newValue);

	std::unique_ptr<Subject>& GetSubject();
private:
	bool m_Falling{ false };
	glm::vec3 fallingSpeed = glm::vec3{ 0, 5, 0 };

	dae::TransformComponent* m_pTransform{nullptr};
	std::unique_ptr<Subject> m_pSubject;

};

