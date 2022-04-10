#include "MiniginPCH.h"
#include "FoodComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "imgui.h"
#include <functional>
#include <RigidbodyComponent.h>
#include "PlayerComponent.h"
#include <GameTime.h>
#include "MrHotDogComponent.h"

FoodComponent::FoodComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
	, m_pTransform(pGameobject->GetComponent<dae::TransformComponent>())
	, m_pSubject(std::make_unique<Subject>())
{
	auto rigidBodyComponent = pGameobject->GetComponent<dae::RigidbodyComponent>();
	std::function<void(dae::RigidbodyComponent* ,dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent*  pTriggeredBody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameobject();
		auto foodComp = pTriggeredBody->GetGameobject()->GetComponent<FoodComponent>();

		if (pOtherGO->GetTag() == "Player")
		{
			//auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			foodComp->SetFalling(true);
		}
		else if (foodComp->GetFalling())
		{
			foodComp->SetFalling(false);
		}
	};

	rigidBodyComponent->SetOnEnterFunction(newFunction);
}

void FoodComponent::Update()
{
	if (m_Falling)
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + fallingSpeed * GameTime::GetInstance()->GetElapsed());
	}
}
#ifdef _DEBUG
void FoodComponent::RenderGUI()
{
	ImGui::Text(("Falling: " + std::to_string(m_Falling)).c_str());
}
#endif // _DEBUG


void FoodComponent::SetFalling(bool newValue)
{
	m_Falling = newValue;
	if (m_Falling)
		m_pSubject->Notify(*m_pGameObject, Event::Burger_Drop);
}

bool FoodComponent::GetFalling() const
{
	return m_Falling;
}

std::unique_ptr<Subject>& FoodComponent::GetSubject()
{
	return m_pSubject;
}
