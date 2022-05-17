#include "EnemySpawnerComponent.h"
#include "MrHotDogComponent.h"
#include "Event.h"

#include <Collider.h>
#include <RigidbodyComponent.h>
#include <Scene.h>
#include <Renderer.h>
#include <GameTime.h>

#include "MrHotDogComponent.h"
#include "ScoreDisplayComponent.h"

dae::Creator<dae::Component, EnemySpawnerComponent> g_EnemySpawnerComponentCreator;

EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
{
	std::cout << "Size creator: " << std::to_string(sizeof(g_EnemySpawnerComponentCreator)) << std::endl;
}

EnemySpawnerComponent::~EnemySpawnerComponent()
{
}

void EnemySpawnerComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	if (action == Event::Enemy_Died)
		m_Count--;
}

void EnemySpawnerComponent::Start()
{
	m_pScoreDisplay = m_pGameObject->GetScene()->FindGameobjectWithTag("Score")->GetComponent<ScoreDisplayComponent>();
}

void EnemySpawnerComponent::Update()
{
	if (m_Count < m_MaxCount && m_Timer >= m_TimebetweenSpawns)
	{
		m_Timer = 0;
		SpawnEnemyAtLocation(m_SpawnPoints[++m_CurrentIndex % m_MaxCount]);
	}

	m_Timer += GameTime::GetInstance()->GetElapsed();
}

void EnemySpawnerComponent::Render() const
{
	for (auto point : m_SpawnPoints)
	{
		dae::Renderer::GetInstance().RenderCircle(point, 2.f);
	}
}

void EnemySpawnerComponent::AddSpawnPosition(glm::vec3 spawnPosition)
{
	m_SpawnPoints.push_back(spawnPosition);
}

void EnemySpawnerComponent::SpawnEnemyAtLocation(glm::vec3 position)
{
	// Spawn Enemy
	m_pGameObject->GetScene()->AddGameObject(CreateMrHotDog(position));

	++m_Count;
}

dae::GameObject* EnemySpawnerComponent::CreateMrHotDog(glm::vec3 position)
{
	auto pHotDogGameObject = new dae::GameObject("MrHotDog");
	pHotDogGameObject->AddComponent(new dae::TransformComponent(pHotDogGameObject, position, glm::vec3{ 2.f }));
	pHotDogGameObject->AddComponent(new dae::SpriteRendererComponent(pHotDogGameObject, "BurgerTime_SpriteSheet.png"));
	pHotDogGameObject->AddComponent(new dae::AnimatorComponent(pHotDogGameObject, "../Data/Animations/HotdogAnimations.json"));
	pHotDogGameObject->AddComponent(new dae::ColliderComponent(pHotDogGameObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));
	pHotDogGameObject->AddComponent(new dae::RigidbodyComponent(pHotDogGameObject));
	auto pEnemyComponent = new EnemyComponent(pHotDogGameObject, nullptr);
	pHotDogGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pHotDogGameObject->SetParent(pHotDogGameObject);
	pHotDogGameObject->SetTag("Enemy");

	return pHotDogGameObject;
}

dae::GameObject* EnemySpawnerComponent::CreateMrEgg(glm::vec3 position)
{
	auto pEggGameObject = new dae::GameObject("MrEgg");
	pEggGameObject->AddComponent(new dae::TransformComponent(pEggGameObject, glm::vec3(288.f, 144.f, 0), glm::vec3{ 2.f }));
	pEggGameObject->AddComponent(new dae::SpriteRendererComponent(pEggGameObject, "BurgerTime_SpriteSheet.png"));
	pEggGameObject->AddComponent(new dae::AnimatorComponent(pEggGameObject, "../Data/Animations/EggAnimations.json"));
	pEggGameObject->AddComponent(new dae::ColliderComponent(pEggGameObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));
	pEggGameObject->AddComponent(new dae::RigidbodyComponent(pEggGameObject));
	auto pEnemyComponent = new EnemyComponent(pEggGameObject, nullptr);
	pEggGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pEggGameObject->SetParent(m_pGameObject);
	pEggGameObject->SetTag("Enemy");
	
	return pEggGameObject;
}
