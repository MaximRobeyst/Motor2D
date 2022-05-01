#include <glm/glm.hpp>
#include <string>
#include <SDL.h>
#include <memory>

#include "FoodComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "imgui.h"
#include <functional>
#include <RigidbodyComponent.h>
#include "PlayerComponent.h"
#include <GameTime.h>
#include "MrHotDogComponent.h"
#include "PlateComponent.h"
#include <ServiceLocator.h>

FoodComponent::FoodComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
	, m_pTransform(pGameobject->GetComponent<dae::TransformComponent>())
	, m_pRigidbody{pGameobject->GetComponent<dae::RigidbodyComponent>()}
	, m_pSubject(std::make_unique<Subject>())
{
	std::function<void(dae::RigidbodyComponent* ,dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent*  pTriggeredBody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameobject();
		auto foodComp = pTriggeredBody->GetGameobject()->GetComponent<FoodComponent>();

		if (pOtherGO->GetTag() == "Player")
		{
			//auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			foodComp->SetFalling(true);
		}
		else if (pOtherGO->GetTag() == "Food")
		{
			//foodComp->SetFalling(false);
			//foodComp->GetSubject()->Notify(*pTriggeredBody->GetGameobject(), Event::Burger_Drop);
			foodComp = pOtherGO->GetComponent<FoodComponent>();
			foodComp->SetFalling(true);
		}
		else if (pOtherGO->GetTag() == "Plate")
		{
			foodComp->SetFalling(false);

			PlateComponent* plateComp{ nullptr };
			if (pOtherGO->GetParent() != nullptr)
				plateComp = pOtherGO->GetParent()->GetComponent<PlateComponent>();
			else
				plateComp = pOtherGO->GetComponent<PlateComponent>();

			plateComp->AddIngredient(pTriggeredBody->GetGameobject());
		}

		else if (foodComp->GetFalling() && pOtherGO->GetTag().empty())
		{
			foodComp->SetFalling(false);
			foodComp->GetSubject()->Notify(*pTriggeredBody->GetGameobject(), Event::Burger_Drop);
			ServiceLocator::GetAudio()->PlaySound("../Data/Audio/eat_ghost.wav");
		}
	};

	m_pRigidbody->SetOnEnterFunction(newFunction);
}

void FoodComponent::Update()
{
	if (m_Falling)
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + glm::vec3{ m_FallingSpeed, 0.f } *GameTime::GetInstance()->GetElapsed());
		m_pRigidbody->GetBody()->SetLinearVelocity(b2Vec2{});
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
	//if (m_Falling)
	//	m_pSubject->Notify(*m_pGameObject, Event::Burger_Drop);
}

bool FoodComponent::GetFalling() const
{
	return m_Falling;
}

std::unique_ptr<Subject>& FoodComponent::GetSubject()
{
	return m_pSubject;
}
