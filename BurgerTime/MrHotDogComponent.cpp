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
#include <string>

#include <Subject.h>
#include "FoodComponent.h"
#include <GameTime.h>

const dae::Creator<dae::Component, EnemyComponent> g_EnemyComponentCreator{};

EnemyComponent::EnemyComponent()
	: dae::Component()
	, m_pSubject{ std::make_unique<Subject>() }
{
}

EnemyComponent::EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform, int score)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>()}
	, m_pPlayerTransform{pPlayerTransform}
	, m_Score{score}
{
}

EnemyComponent::~EnemyComponent()
{
	delete m_pEnemyStateMachine;
}

void EnemyComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();
	m_pColliderComponent = m_pGameObject->GetComponent < dae::ColliderComponent>();

	m_pAnimatorComponent->SetAnimation("WalkLeft");
	m_CurrentTarget = glm::vec2{ m_pTransformComponent->GetPosition() };

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [this](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
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
			if (m_Dead)
				return;

			auto foodcomp = pOtherGO->GetComponent<FoodComponent>();

			if (foodcomp->GetFalling())
				enemyComp->EnemyDeath();
			else
			{
				foodcomp->AddEnemy(m_pGameObject);
			}
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

	if (m_pEnemyStateMachine != nullptr) return;

	const auto pLeftState = new LeftState(this, m_pPlayerTransform);
	const auto pUpState = new UpState(this, m_pPlayerTransform);
	const auto pRightState = new RightState(this, m_pPlayerTransform);
	const auto pDownState = new DownState(this, m_pPlayerTransform);

	m_pEnemyStateMachine = new EnemyStateMachine(this, pLeftState);

	m_pEnemyStateMachine->AddState(pUpState);
	m_pEnemyStateMachine->AddState(pDownState);
	m_pEnemyStateMachine->AddState(pRightState);

	if (!SpaceLeft())
		m_pEnemyStateMachine->SwitchGameState(pRightState);

	// From left state to up or down
	m_pEnemyStateMachine->AddTransition(pLeftState, pUpState, [&]()
		{
			float playerY = m_pPlayerTransform->GetPosition().y;
			float enemyY = m_pTransformComponent->GetPosition().y;

			return SpaceUp() && (abs(playerY - enemyY) > 1.f && playerY < enemyY);
		});

	m_pEnemyStateMachine->AddTransition(pLeftState, pDownState, [&]()
		{
			float playerY = m_pPlayerTransform->GetPosition().y;
			float enemyY = m_pTransformComponent->GetPosition().y;

			return SpaceDown() && (abs(playerY - enemyY) > 1.f && playerY > enemyY);
		});

	// From up to left or right
	m_pEnemyStateMachine->AddTransition(pUpState, pLeftState, [&]()
		{
			float playerX = m_pPlayerTransform->GetPosition().x;
			float enemyX = m_pTransformComponent->GetPosition().x;

			return SpaceLeft() && playerX < enemyX;
		});

	m_pEnemyStateMachine->AddTransition(pUpState, pRightState, [&]()
		{
			float playerX = m_pPlayerTransform->GetPosition().x;
			float enemyX = m_pTransformComponent->GetPosition().x;

			return SpaceRight() && playerX > enemyX;
		});

	// From down to left or right
	m_pEnemyStateMachine->AddTransition(pDownState, pLeftState, [&]()
		{
			float playerX = m_pPlayerTransform->GetPosition().x;
			float enemyX = m_pTransformComponent->GetPosition().x;

			return SpaceLeft() && playerX < enemyX;
		});

	m_pEnemyStateMachine->AddTransition(pDownState, pRightState, [&]()
		{
			float playerX = m_pPlayerTransform->GetPosition().x;
			float enemyX = m_pTransformComponent->GetPosition().x;

			return SpaceRight() && playerX > enemyX;
		});

	// From right to up or down
	m_pEnemyStateMachine->AddTransition(pRightState, pUpState, [&]()
		{
			float playerY = m_pPlayerTransform->GetPosition().y;
			float enemyY = m_pTransformComponent->GetPosition().y;

			return SpaceUp() && (abs(playerY - enemyY) > 1.f && playerY < enemyY);
		});

	m_pEnemyStateMachine->AddTransition(pRightState, pDownState, [&]()
		{
			float playerY = m_pPlayerTransform->GetPosition().y;
			float enemyY = m_pTransformComponent->GetPosition().y;

			return SpaceDown() && (abs(playerY - enemyY) > 1.f && playerY > enemyY);
		});

}

void EnemyComponent::Update()
{
	if (m_Dead)
	{
		if (m_pAnimatorComponent->IsAnimationDone())
			m_pGameObject->SetEnabled(false);
		return;
	}

	m_Timer += GameTime::GetInstance()->GetElapsed();
	if (m_Timer >= m_TimeBetweenTransitionChecks)
	{
		m_pEnemyStateMachine->UpdateTransitions();
		m_Timer -= m_TimeBetweenTransitionChecks;
	}

	m_pEnemyStateMachine->Update();
}

void EnemyComponent::Render() const
{
	m_pEnemyStateMachine->Render();

	SpaceDown();
	SpaceUp();
	SpaceLeft();
	SpaceRight();
}

void EnemyComponent::RenderGUI()
{
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

void EnemyComponent::EnemyDrop()
{
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

bool EnemyComponent::SpaceUp() const
{
	dae::RaycastCallback callback{ static_cast<uint16>(dae::PhysicsLayers::layer1) };
	const auto world = GetGameObject()->GetScene()->GetPhysicsWorld();

	b2Vec2 startPosition
	{
		m_pTransformComponent->GetPosition().x,
		m_pTransformComponent->GetPosition().y + (m_pColliderComponent->GetSize().y / 2.f)
	};
	b2Vec2 endPosition{};
	const int segments = 2;

	for (int i = 0; i <= segments; ++i)
	{
		endPosition = startPosition + b2Vec2{ 0.f, -m_pColliderComponent->GetSize().y };

		world->RayCast(&callback, startPosition, endPosition);	// Save world in member variable
		if (callback.GetLatestHit().hit)
			return false;

		dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, callback.GetLatestHit().hit ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 0, 255, 0, 255 });
		startPosition.x += (m_pColliderComponent->GetSize().x / segments);
	}

	return true;
}

bool EnemyComponent::SpaceDown() const
{
	dae::RaycastCallback callback{static_cast<uint16>(dae::PhysicsLayers::layer1)};
	const auto world = GetGameObject()->GetScene()->GetPhysicsWorld();

	b2Vec2 startPosition
	{
		m_pTransformComponent->GetPosition().x,
		m_pTransformComponent->GetPosition().y + (m_pColliderComponent->GetSize().y / 2.f)
	};
	b2Vec2 endPosition{};
	const int segments = 2;

	for (int i = 0; i <= segments; ++i)
	{
		endPosition = startPosition + b2Vec2{ 0.f, m_pColliderComponent->GetSize().y };

		world->RayCast(&callback, startPosition, endPosition);	// Save world in member variable
		if (callback.GetLatestHit().hit)
			return false;

		dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, callback.GetLatestHit().hit ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 0, 255, 0, 255 });
		startPosition.x += (m_pColliderComponent->GetSize().x / segments);
	}

	return true;
}

bool EnemyComponent::SpaceLeft() const
{
	dae::RaycastCallback callback{ static_cast<uint16>(dae::PhysicsLayers::layer1) };
	const auto world = GetGameObject()->GetScene()->GetPhysicsWorld();

	b2Vec2 startPosition
	{
		m_pTransformComponent->GetPosition().x + (m_pColliderComponent->GetSize().x / 2.f),
		m_pTransformComponent->GetPosition().y
	};
	b2Vec2 endPosition{};
	const int segments = 2;
	for (int i = 0; i <= segments; ++i)
	{
		endPosition = startPosition + b2Vec2{ -m_pColliderComponent->GetSize().x ,0.f };

		world->RayCast(&callback, startPosition, endPosition);	// Save world in member variable
		if (callback.GetLatestHit().hit)
			return false;

		dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, callback.GetLatestHit().hit ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 0, 255, 0, 255 });
		startPosition.y += (m_pColliderComponent->GetSize().y / segments);
	}
	return true;
}

bool EnemyComponent::SpaceRight() const
{
	dae::RaycastCallback callback{ static_cast<uint16>(dae::PhysicsLayers::layer1) };
	const auto world = GetGameObject()->GetScene()->GetPhysicsWorld();

	b2Vec2 startPosition
	{
		m_pTransformComponent->GetPosition().x + (m_pColliderComponent->GetSize().x / 2.f),
		m_pTransformComponent->GetPosition().y
	};
	b2Vec2 endPosition{};
	const int segments = 2;
	for (int i = 0; i <= segments; ++i)
	{
		endPosition = startPosition + b2Vec2{ m_pColliderComponent->GetSize().x ,0.f };

		world->RayCast(&callback, startPosition, endPosition);	// Save world in member variable
		if (callback.GetLatestHit().hit)
			return false;

		dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, callback.GetLatestHit().hit ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 0, 255, 0, 255 });
		startPosition.y += (m_pColliderComponent->GetSize().y / segments);
	}
	return true;
}

EnemyStateMachine::EnemyStateMachine(EnemyComponent* pEnemyComponent, IEnemyState* pEnemyState)
	: m_pEnemyComponent{pEnemyComponent}
	, m_pCurrentState{pEnemyState}
{
	m_pStates.push_back(pEnemyState);

	if (m_pCurrentState == nullptr) return;
	m_pCurrentState->OnEnter();
}

EnemyStateMachine::~EnemyStateMachine()
{
	for (auto state : m_pStates)
	{
		delete state;
	}

	m_pStates.clear();
}

void EnemyStateMachine::SwitchGameState(IEnemyState* pGameState)
{
	if (m_pCurrentState)
		m_pCurrentState->OnExit();

	m_pCurrentState = pGameState;

	if (m_pCurrentState)
		m_pCurrentState->OnEnter();
}

void EnemyStateMachine::AddState(IEnemyState* pState)
{
	m_pStates.push_back(pState);
}

void EnemyStateMachine::AddTransition(IEnemyState* pFromState, IEnemyState* pToState, std::function<bool()> condition)
{
	auto it = m_pTransitions.find(pFromState);
	if (it == m_pTransitions.end())
	{
		m_pTransitions[pFromState] = std::vector<TransitionPair>();
	}

	m_pTransitions[pFromState].push_back(std::make_pair(pToState, condition));
}

void EnemyStateMachine::UpdateTransitions()
{
	if (m_pCurrentState == nullptr) return;

	if (m_pTransitions[m_pCurrentState].size() != 0)
	{
		for (size_t i = 0; i < m_pTransitions[m_pCurrentState].size(); ++i)
		{
			if (m_pTransitions[m_pCurrentState][i].second())
				SwitchGameState(m_pTransitions[m_pCurrentState][i].first);
		}
	}
}

void EnemyStateMachine::Update()
{
	if (m_pCurrentState == nullptr) return;
	m_pCurrentState->Update();
}

void EnemyStateMachine::Render()
{
	if (m_pCurrentState == nullptr) return;
	m_pCurrentState->Render();
}

LeftState::LeftState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform)
	: IEnemyState{ pEnemyComponent }
	, m_pPlayertransform{pPlayerTransform}
{
	m_pRigidbodyComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::RigidbodyComponent>();
	m_pTransformComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::TransformComponent>();
	m_pColliderComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::ColliderComponent>();
}

void LeftState::OnEnter()
{
}

void LeftState::Update()
{

	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ -m_Speed, 0.f });
}

void LeftState::Render()
{
}

void LeftState::OnExit()
{
}

IEnemyState::IEnemyState(EnemyComponent* pEnemyComponent)
	: m_pEnemyComponent{pEnemyComponent}
{
}

UpState::UpState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform)
	: IEnemyState{ pEnemyComponent }
	, m_pPlayertransform{ pPlayerTransform }
	, m_pWorld{ pEnemyComponent->GetGameObject()->GetScene()->GetPhysicsWorld() }
{
	m_pRigidbodyComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::RigidbodyComponent>();
	m_pTransformComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::TransformComponent>();
	m_pColliderComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::ColliderComponent>();
}

void UpState::OnEnter()
{
}

void UpState::Update()
{
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, -m_Speed });
}

void UpState::Render()
{
}

void UpState::OnExit()
{
}

DownState::DownState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform)
	: IEnemyState{ pEnemyComponent }
	, m_pPlayertransform{ pPlayerTransform }
{
	m_pRigidbodyComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::RigidbodyComponent>();
	m_pTransformComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::TransformComponent>();
}

void DownState::OnEnter()
{
}

void DownState::Update()
{
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, m_Speed });
}

void DownState::Render()
{
}

void DownState::OnExit()
{
}

RightState::RightState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform)
	: IEnemyState{ pEnemyComponent }
	, m_pPlayertransform{ pPlayerTransform }
{
	m_pRigidbodyComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::RigidbodyComponent>();
	m_pTransformComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::TransformComponent>();
	m_pColliderComponent = m_pEnemyComponent->GetGameObject()->GetComponent<dae::ColliderComponent>();
}

void RightState::OnEnter()
{
}

void RightState::Update()
{
	if (m_pRigidbodyComponent->GetBody()->GetLinearVelocity() == b2Vec2{ 0.f, 0.f })
	{
		m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, m_Speed });
	}

	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ m_Speed, 0.f });
}

void RightState::Render()
{
}

void RightState::OnExit()
{
}
