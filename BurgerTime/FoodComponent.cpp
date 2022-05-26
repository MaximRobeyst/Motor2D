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
#include <Collider.h>

dae::Creator<dae::Component, FoodComponent> s_TranformComponentCreate{};

FoodComponent::FoodComponent()
	:Component()
	, m_pSubject(std::make_unique<Subject>())
{
}

FoodComponent::FoodComponent(dae::GameObject* pGameobject, bool topPart)
	: Component(pGameobject)
	, m_pSubject(std::make_unique<Subject>())
	, m_TopPart{topPart}
{
}

void FoodComponent::Start()
{
	m_pCollider = m_pGameObject->GetComponent<dae::ColliderComponent>();
	m_pTransform = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbody = m_pGameObject->GetComponent<dae::RigidbodyComponent>();

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> enterFunction = [](dae::RigidbodyComponent* pTriggeredBody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();
		auto foodComp = pTriggeredBody->GetGameObject()->GetComponent<FoodComponent>();

		if (pOtherGO->GetTag() == "Player")
		{
			foodComp->SetCollidingWithPlayer(true, pOtherGO);
			//foodComp->SetFalling(true);
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

			plateComp->AddIngredient(pTriggeredBody->GetGameObject()->GetComponent<FoodComponent>());
		}

		else if (foodComp->GetFalling() && pOtherGO->GetTag().empty())
		{
			foodComp->SetFalling(false);

			foodComp->GetSubject()->Notify(*pTriggeredBody->GetGameObject(), Event::Burger_Drop);
			ServiceLocator::GetAudio()->PlaySound("../Data/Audio/eat_ghost.wav");
		}
	};

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> exitFunction = [](dae::RigidbodyComponent* pTriggeredBody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();
		auto foodComp = pTriggeredBody->GetGameObject()->GetComponent<FoodComponent>();

		if (pOtherGO->GetTag() == "Player")
		{
			foodComp->SetCollidingWithPlayer(false, nullptr);
		}
	};

	m_pRigidbody->SetOnEnterFunction(enterFunction);
	m_pRigidbody->SetOnExitFunction(exitFunction);
}

void FoodComponent::Update()
{
	if (m_CollidingWithPlayer)
		CollidingWithPlayer();

	if (m_Falling)
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + glm::vec3{ m_FallingSpeed, 0.f } *GameTime::GetInstance()->GetElapsed());
		m_pRigidbody->GetBody()->SetLinearVelocity(b2Vec2{});
	}
	else
	{
		bool fall {true};
		for (int i = 0; i < m_Parts; ++i)
		{
			if (!m_PartStates[i])
			{
				fall = false;
				break;
			}
		}
		if (fall)
			SetFalling(true);
	}
}
void FoodComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("TopPart");
	writer.Bool(m_TopPart);
	writer.EndObject();
}
void FoodComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_TopPart = value["TopPart"].GetBool();

}
#ifdef _DEBUG
void FoodComponent::RenderGUI()
{
	ImGui::Text(("Falling: " + std::to_string(m_Falling)).c_str());
}
#endif // _DEBUG


void FoodComponent::SetFalling(bool newValue)
{
	if (!newValue)
	{
		for (int i = 0; i < m_Parts; ++i)
		{
			m_PartStates[i] = false;
		}
	}

	m_Falling = newValue;

	//if (m_Falling)
	//	m_pSubject->Notify(*m_pGameObject, Event::Burger_Drop);
}

bool FoodComponent::GetFalling() const
{
	return m_Falling;
}

void FoodComponent::SetState(int i, bool newState)
{
	assert(i < m_Parts);
	m_PartStates[i] = true;
}

void FoodComponent::SetCollidingWithPlayer(bool newState, dae::GameObject* pGameobject)
{
	m_CollidingWithPlayer = newState;
	m_pPlayer = pGameobject;
}

bool FoodComponent::IsTop() const
{
	return m_TopPart;
}

std::unique_ptr<Subject>& FoodComponent::GetSubject()
{
	return m_pSubject;
}

void FoodComponent::CollidingWithPlayer()
{
	assert(m_pPlayer != nullptr);

	auto colliderComp = m_pGameObject->GetComponent<dae::ColliderComponent>();
	auto transform = m_pGameObject->GetComponent<dae::TransformComponent>();

	//auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
	float width = colliderComp->GetSize().x;
	float playerX = m_pPlayer->GetComponent<dae::TransformComponent>()->GetPosition().x;
	float startX = transform->GetPosition().x - (colliderComp->GetSize().x / 2);
	float partWidths = width / m_Parts;

	for (int i = 0; i < m_Parts; ++i)
	{
		if (playerX > (startX + (partWidths * i)) && playerX < (startX + (partWidths * (i + 1))))
		{
			if (m_PartStates[i] == false)
			{
				ServiceLocator::GetAudio()->PlaySound("../Data/Audio/munch_" + std::to_string(i % 2 + 1) + ".wav");
				m_PartStates[i] = true;
			}

		}
	}
}
