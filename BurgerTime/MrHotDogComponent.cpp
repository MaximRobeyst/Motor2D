#include "MiniginPCH.h"
#include "MrHotDogComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "PlayerComponent.h"
#include "Event.h"

#include <Scene.h>
#include <ServiceLocator.h>
#include <RaycastCallback.h>
#include <Renderer.h>
#include <AnimatorComponent.h>
#include <Collider.h>

#include <Subject.h>
#include "FoodComponent.h"

dae::Creator<dae::Component, EnemyComponent> s_TranformComponentCreate{};

EnemyComponent::EnemyComponent()
	: dae::Component()
	, m_pSubject{ std::make_unique<Subject>() }
{
}

EnemyComponent::EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>()}
	, m_pPlayerTransform{pPlayerTransform}
{
}

void EnemyComponent::Start()
{
	m_pTransfomComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();
	m_pColliderComponent = m_pGameObject->GetComponent < dae::ColliderComponent>();

	m_pAnimatorComponent->SetAnimation("WalkLeft");
	m_CurrentTarget = glm::vec2{ m_pTransfomComponent->GetPosition() };

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Food")
		{
			auto enemyComp = pTriggeredbody->GetGameObject()->GetComponent<EnemyComponent>();
			auto foodcomp = pOtherGO->GetComponent<FoodComponent>();

			if(foodcomp->GetFalling())
				enemyComp->EnemyDeath();
		}
	};

	m_pRigidbodyComponent->SetOnEnterFunction(newFunction);

	if (m_pPlayerTransform == nullptr)
	{
		auto pPlayerobject = m_pGameObject->GetScene()->FindGameobjectWithTag("Player");
		if (pPlayerobject != nullptr)
		{
			m_pPlayerTransform = pPlayerobject->GetComponent<dae::TransformComponent>();
		}
	}

	//auto pColliderTop = new dae::GameObject();
	//m_pRigidbodyComponent = new dae::RigidbodyComponent();
}

void EnemyComponent::Update()
{
	UpdateWalk();

	if (m_Dead)
	{
		if (m_pAnimatorComponent->IsAnimationDone())
			dae::SceneManager::GetInstance().GetScene(0)->RemoveGameObject(m_pGameObject);

		return;
	}

	//if(m_pPlayerTransform->GetPosition().y > 0)

	//if (m_pPlayerTransform == nullptr) return;
	//if (m_pPlayerTransform->GetPosition().x <= m_pTransfomComponent->GetPosition().x)
	//	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ -m_Speed, 0.f });
	//else if(m_pPlayerTransform->GetPosition().x >= m_pTransfomComponent->GetPosition().x)
	//	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ m_Speed, 0.f });

}

void EnemyComponent::Render() const
{
	dae::Renderer::GetInstance().RenderCircle(m_CurrentTarget, 1.f);
	//dae::Renderer::GetInstance().RenderLine(m_pTransfomComponent->GetPosition(), glm::vec2{ m_pTransfomComponent->GetPosition() }, SDL_Color{ 255, 255, 0, 255 });


	b2Vec2 startPosition{
		m_pTransfomComponent->GetPosition().x + (m_pColliderComponent->GetSize().x / 2.0f),
		m_pTransfomComponent->GetPosition().y + (m_pColliderComponent->GetSize().y / 2.0f),
	};
	float rayDistance = m_pColliderComponent->GetSize().x;

	b2Vec2 directions[4] = { b2Vec2{0,-1 * rayDistance}, b2Vec2{-1 * rayDistance,0}, b2Vec2{0, 1 * rayDistance}, b2Vec2{1 * rayDistance, 0} };

	for (int i = 0; i < 4; ++i)
	{
		b2Vec2 endPosition = startPosition + directions[i];
		dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, SDL_Color{ 255, 0, 255, 255 });

	}

	dae::Renderer::GetInstance().RenderCircle(m_CurrentTarget, 10.f, SDL_Color{ 255, 255, 0, 255 });
	dae::Renderer::GetInstance().RenderCircle(m_pPlayerTransform->GetPosition(), 10.f, SDL_Color{255, 255, 0, 255});

}

void EnemyComponent::RenderGUI()
{
	float target[2] = { m_CurrentTarget.x, m_CurrentTarget.y };
	ImGui::InputFloat2("CurrentDirection", target);
	float position[2] = { m_pTransfomComponent->GetPosition().x, m_pTransfomComponent->GetPosition().y };
	ImGui::InputFloat2("CurrentPosition", position);
	float playerPosition[2] = { m_pPlayerTransform->GetPosition().x, m_pPlayerTransform->GetPosition().y };
	ImGui::InputFloat2("CurrentPosition", playerPosition);

	ImGui::InputFloat("minimum distance", &m_MinDistance);
}

void EnemyComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void EnemyComponent::Deserialize(dae::GameObject* pGameObject, rapidjson::Value&)
{
	m_pGameObject = pGameObject;
}

void EnemyComponent::EnemyDeath()
{
	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/death_2.wav");
	m_pAnimatorComponent->SetAnimation("Death");
	m_pSubject->Notify(*m_pGameObject, Event::Enemy_Died);
	m_Dead = true;
}

int EnemyComponent::GetScore() const
{
	return m_Score;
}

std::unique_ptr<Subject>& EnemyComponent::GetSubject()
{
	return m_pSubject;
}

void EnemyComponent::UpdateWalk()
{
	auto difference = (glm::vec2{ m_pTransfomComponent->GetPosition() } - m_CurrentTarget);
	float distance = powf(difference.x, 2) + powf(difference.y, 2);
	if ( distance <= m_MinDistance)
	{
		ChooseNextTarget();
	}

	difference = (m_PreviousPosition -m_pTransfomComponent->GetPosition());
	distance = powf(difference.x, 2) + powf(difference.y, 2);

	if (distance <= 0.0f)
	{
		ChooseNextTarget();
	}

	glm::vec2 currentDirection = m_CurrentTarget - glm::vec2{ m_pTransfomComponent->GetPosition() };
	currentDirection = glm::normalize(currentDirection) * m_Speed;
	
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ currentDirection.x, currentDirection.y });
	m_PreviousPosition = m_pTransfomComponent->GetPosition();

	// Raycast up to check if we can climb a ladder
	//bool spaceAbove = SpaceAbove();
	////bool spaceBelow = SpaceBelow();
	//
	////std::cout << "My position ["
	////	<< m_pTransfomComponent->GetPosition().x << " , " << m_pTransfomComponent->GetPosition().y << " ]" << std::endl << 
	////	"The players Position[" << m_pPlayerTransform->GetPosition().x << " , " << m_pPlayerTransform->GetPosition().y << " ] " << std::endl << 
	////	"Space above head: ?" << spaceAbove << " " << std::endl;
	//
	//if (spaceAbove && fabsf((m_pTransfomComponent->GetPosition().y - m_pPlayerTransform->GetPosition().y)) > 1.0f)
	//{
	//	//m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, -m_Speed });
	//	m_CurrentState = EnemyState::Climb;
	//	return;
	//}
	
}

void EnemyComponent::UpdatePlayerDeath()
{
}

void EnemyComponent::ChooseNextTarget()
{
	glm::vec3 newDirection;
	b2Vec2 startPosition{
		m_pTransfomComponent->GetPosition().x + (m_pColliderComponent->GetSize().x / 2.0f),
		m_pTransfomComponent->GetPosition().y + (m_pColliderComponent->GetSize().y / 2.0f),
	};
	dae::Renderer::GetInstance().RenderCircle(startPosition, 1.0f);

	glm::vec2 potentialPosition[4];
	glm::vec2 directions[4] = { 
		glm::vec2{0,-1 * (m_pColliderComponent->GetSize().y) }, glm::vec2{-1 * (m_pColliderComponent->GetSize().x) ,0},
		glm::vec2{0, 1 * (m_pColliderComponent->GetSize().y) }, glm::vec2{1 * (m_pColliderComponent->GetSize().x) , 0}
	};
	int indexForShortestDistance{};
	float shortestDistance{ FLT_MAX };

	dae::RaycastCallback raycastCallback{};

	for (int i = 0; i < 4; ++i)
	{
		potentialPosition[i] = glm::vec2{ m_pTransfomComponent->GetPosition() } + (directions[i]);
		potentialPosition[i] = glm::vec2{ roundf(potentialPosition[i].x), roundf(potentialPosition[i].y) };

		float currentDistance = glm::distance(potentialPosition[i], glm::vec2{ m_pPlayerTransform->GetPosition() });

		if (currentDistance <= shortestDistance)
		{
			dae::Renderer::GetInstance().RenderCircle(potentialPosition[i], 10.f, SDL_Color{ 255, 255, 0, 255 });
			b2Vec2 endPosition{ startPosition.x + directions[i].x , startPosition.y + directions[i].y };
			m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition, endPosition);
			if (raycastCallback.GetLatestHit().pHitObject == nullptr)
			{
				shortestDistance = glm::distance(glm::vec2{ m_pPlayerTransform->GetPosition() }, potentialPosition[indexForShortestDistance]);
				indexForShortestDistance = i;
			}
		}
	}

	m_CurrentTarget = potentialPosition[indexForShortestDistance];
}

bool EnemyComponent::SpaceAbove()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y + 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,-3.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / (sections- 1.f);

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ sizeDevidedBySections * i, 0.f }, endPosition + b2Vec2{ sizeDevidedBySections * i, 0.f });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceBelow()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y + 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,-2.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition +b2Vec2{sizeDevidedBySections * i, 0.f}, endPosition + b2Vec2{sizeDevidedBySections * i, 0.f});
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceLeft()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y - 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,2.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ sizeDevidedBySections * i, 0.f }, endPosition + b2Vec2{ sizeDevidedBySections * i, 0.f });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceRight()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x + 1.0f, pos.y };
	b2Vec2 endPosition{ startPosition + b2Vec2{-2.f, 0.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ 0.f, sizeDevidedBySections * i}, endPosition + b2Vec2{ 0.f,  sizeDevidedBySections * i });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}
