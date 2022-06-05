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
#include <SpriteRendererComponent.h>
#include <Subject.h>
#include <AnimatorComponent.h>
#include <GameObject.h>

const dae::Creator<dae::Component, EnemySpawnerComponent> g_EnemySpawnerComponentCreator;

EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
{
}

EnemySpawnerComponent::~EnemySpawnerComponent()
{
}

void EnemySpawnerComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	if (action == Event::Enemy_Died)
		--m_Count;
}

void EnemySpawnerComponent::Start()
{
	SetId(m_pGameObject->GetId());
	m_pScoreDisplay = m_pGameObject->GetScene()->FindGameobjectWithTag("Score")->GetComponent<ScoreDisplayComponent>();
	for (int i = 0; i < m_MaxCount; ++i)
	{
		SpawnEnemyAtLocation(m_SpawnPoints[i]);
	}
}

void EnemySpawnerComponent::Update()
{
	
	if (m_Count < m_MaxCount && m_Timer >= m_TimebetweenSpawns)
	{
		m_Timer = 0;
		++m_Count;

		// find disabled object 
		auto iter = std::find_if(m_SpawnedObjects.begin(), m_SpawnedObjects.end(), [](dae::GameObject* pGameobject) {
				return !pGameobject->IsEnabled();
			});
		if (iter == m_SpawnedObjects.end())
			return;

		(*iter)->GetComponent<dae::TransformComponent>()->SetPosition(m_SpawnPoints[m_CurrentIndex]);
		++m_CurrentIndex %= m_MaxCount;

		(*iter)->SetEnabled(true);

	}

	m_Timer += GameTime::GetInstance()->GetElapsed();
}

void EnemySpawnerComponent::Render() const
{
	for (auto& point : m_SpawnPoints)
	{
		dae::Renderer::GetInstance().RenderCircle(point, 2.f, SDL_Color{255, 0, 0, 255});
	}
}

void EnemySpawnerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("MaxCount");
	writer.Int(m_MaxCount);
	writer.Key("SpawnPoints");
	writer.StartArray();
	for (const auto& spawnPoint : m_SpawnPoints)
	{
		writer.StartObject();

		writer.Key("X");
		writer.Double(static_cast<double>(spawnPoint.x));
		writer.Key("Y");
		writer.Double(static_cast<double>(spawnPoint.y));
		writer.Key("Z");
		writer.Double(static_cast<double>(spawnPoint.z));

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("TimeBetweenSpawns");
	writer.Double(static_cast<double>(m_TimebetweenSpawns));



	writer.EndObject();
}

void EnemySpawnerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;

	m_MaxCount = value["MaxCount"].GetInt();
	
	int size = value["SpawnPoints"].GetArray().Size();
	for (int i = 0; i < size; ++i)
	{
		auto &pos = value["SpawnPoints"].GetArray()[i];

		m_SpawnPoints.push_back( glm::vec3{
			static_cast<float>(pos["X"].GetDouble()),
			static_cast<float>(pos["Y"].GetDouble()),
			static_cast<float>(pos["Z"].GetDouble())
		});
	}

	m_TimebetweenSpawns = static_cast<float>(value["TimeBetweenSpawns"].GetDouble());
}

void EnemySpawnerComponent::AddSpawnPosition(glm::vec3 spawnPosition)
{
	m_SpawnPoints.push_back(spawnPosition);
}

void EnemySpawnerComponent::SpawnEnemyAtLocation(glm::vec3 position)
{
	// Spawn Enemy
	m_pGameObject->GetScene()->AddGameObject(CreateMrHotDog(position));
}

dae::GameObject* EnemySpawnerComponent::CreateMrHotDog(glm::vec3 position)
{
	auto pHotDogGameObject = new dae::GameObject("MrHotDog");
	pHotDogGameObject->SetTag("Enemy");
	pHotDogGameObject->AddComponent(new dae::TransformComponent(pHotDogGameObject, position, glm::vec3{ 2.f }));
	pHotDogGameObject->AddComponent(new dae::SpriteRendererComponent(pHotDogGameObject, "BurgerTime_SpriteSheet.png"));
	pHotDogGameObject->AddComponent(new dae::AnimatorComponent(pHotDogGameObject, "../Data/Animations/HotdogAnimations.json"));
	pHotDogGameObject->AddComponent(new dae::ColliderComponent(pHotDogGameObject, 14.f, 14.f, glm::vec2{ 7.f, 7.f }));

	uint16 mask = 0;
	mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer1);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);

	pHotDogGameObject->AddComponent(new dae::RigidbodyComponent(pHotDogGameObject, b2_dynamicBody, 1.0f, 1.0f, false, dae::PhysicsLayers::layer2, mask));
	auto pEnemyComponent = new EnemyComponent(pHotDogGameObject, nullptr);
	pHotDogGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pEnemyComponent->GetSubject()->AddObserver(this);
	pHotDogGameObject->SetParent(m_pGameObject);

	pHotDogGameObject->SetSerializable(false);
	pHotDogGameObject->SetEnabled(false);
	m_SpawnedObjects.push_back(pHotDogGameObject);

	return pHotDogGameObject;
}

dae::GameObject* EnemySpawnerComponent::CreateMrEgg(glm::vec3 position)
{
	auto pEggGameObject = new dae::GameObject("MrEgg");
	pEggGameObject->AddComponent(new dae::TransformComponent(pEggGameObject, position, glm::vec3{ 2.f }));
	pEggGameObject->AddComponent(new dae::SpriteRendererComponent(pEggGameObject, "BurgerTime_SpriteSheet.png"));
	pEggGameObject->AddComponent(new dae::AnimatorComponent(pEggGameObject, "../Data/Animations/EggAnimations.json"));
	pEggGameObject->AddComponent(new dae::ColliderComponent(pEggGameObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));

	uint16 mask = 0;
	mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer1);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);

	pEggGameObject->AddComponent(new dae::RigidbodyComponent(pEggGameObject, b2_dynamicBody, 1.0f, 1.0f, false, dae::PhysicsLayers::layer2, mask));
	auto pEnemyComponent = new EnemyComponent(pEggGameObject, nullptr, 100);
	pEggGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pEnemyComponent->GetSubject()->AddObserver(this);
	pEggGameObject->SetParent(m_pGameObject);
	pEggGameObject->SetTag("Enemy");
	pEggGameObject->SetSerializable(false);
	pEggGameObject->SetEnabled(false);
	m_SpawnedObjects.push_back(pEggGameObject);
	
	return pEggGameObject;
}

dae::GameObject* EnemySpawnerComponent::CreateMrPickle(glm::vec3 position)
{
	auto pPickleObject = new dae::GameObject("MrPickle");
	pPickleObject->AddComponent(new dae::TransformComponent(pPickleObject, position, glm::vec3{ 2.f }));
	pPickleObject->AddComponent(new dae::SpriteRendererComponent(pPickleObject, "BurgerTime_SpriteSheet.png"));
	pPickleObject->AddComponent(new dae::AnimatorComponent(pPickleObject, "../Data/Animations/PickleAnimations.json"));
	pPickleObject->AddComponent(new dae::ColliderComponent(pPickleObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));

	uint16 mask = 0;
	mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer1);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);

	pPickleObject->AddComponent(new dae::RigidbodyComponent(pPickleObject, b2_dynamicBody, 1.0f, 1.0f, false, dae::PhysicsLayers::layer2, mask));
	auto pEnemyComponent = new EnemyComponent(pPickleObject, nullptr, 200);
	pPickleObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pEnemyComponent->GetSubject()->AddObserver(this);
	pPickleObject->SetParent(m_pGameObject);
	pPickleObject->SetTag("Enemy");
	pPickleObject->SetSerializable(false);
	pPickleObject->SetEnabled(false);
	m_SpawnedObjects.push_back(pPickleObject);

	return pPickleObject;
}
