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
#include <Utils.h>

#include <RapidjsonHelpers.h>
#include <ResourceManager.h>

const dae::Creator<dae::Component, EnemySpawnerComponent> g_EnemySpawnerComponentCreator;

EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
{
	dae::ResourceManager::GetInstance().LoadTexture("Sprites/Enemy.png");
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
	m_pScoreDisplay = m_pGameObject->GetScene()->FindGameobjectWithTag("Score")->GetComponent<ScoreDisplayComponent>();
	for (int i = 0; i < m_AmountOfEnemies; ++i)
	{
		SpawnEnemy(glm::vec3{
			Random(m_BorderOffset, dae::Renderer::GetInstance().GetWindowWidth() - m_BorderOffset),
			Random(m_BorderOffset, dae::Renderer::GetInstance().GetWindowHeight() - m_BorderOffset), 0}
		)->SetParent(m_pGameObject);
	}
}

void EnemySpawnerComponent::Update()
{
	if (m_Count < m_MaxCount && m_Timer >= m_TimebetweenSpawns)
	{
	
		// find disabled object 
		auto iter = std::find_if(m_SpawnedObjects.begin(), m_SpawnedObjects.end(), [](dae::GameObject* pGameobject) {
				return !pGameobject->IsEnabled();
			});
		if (iter == m_SpawnedObjects.end())
			return;
	
		(*iter)->GetComponent<dae::TransformComponent>()->SetPosition(glm::vec3{
			Random(m_BorderOffset, dae::Renderer::GetInstance().GetWindowWidth() - m_BorderOffset),
			Random(m_BorderOffset, dae::Renderer::GetInstance().GetWindowHeight() - m_BorderOffset), 0 }
		);
		++m_CurrentIndex %= m_MaxCount;
	
		(*iter)->SetEnabled(true);
		(*iter)->GetComponent<EnemyComponent>()->Respawn();
		m_Timer = 0;
		++m_Count;
	
	}
	
	m_Timer += GameTime::GetInstance()->GetElapsed();
}

void EnemySpawnerComponent::Render() const
{
}

void EnemySpawnerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	rapidjson::SerializeValue(writer, "Name", typeid(*this).name());
	rapidjson::SerializeValue(writer, "MaxCount", m_MaxCount);

	rapidjson::SerializeValue(writer, "TimeBetweenSpawns", m_TimebetweenSpawns);
	rapidjson::SerializeValue(writer, "AmountOfEnemies", m_AmountOfEnemies);

	writer.EndObject();
}

void EnemySpawnerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;

	rapidjson::DeserializeValue(value, "MaxCount", m_MaxCount);
	rapidjson::DeserializeValue(value, "TimeBetweenSpawns", m_TimebetweenSpawns);
}

dae::GameObject* EnemySpawnerComponent::SpawnEnemy(glm::vec3 position)
{
	auto pEnemyGameobject = new dae::GameObject("Enemy");
	pEnemyGameobject->SetTag("Enemy");
	pEnemyGameobject->AddComponent(new dae::TransformComponent(pEnemyGameobject, position, glm::vec3{ 0.25f }));
	pEnemyGameobject->AddComponent(new dae::SpriteRendererComponent(pEnemyGameobject, "Sprites/Enemy.png"));
	pEnemyGameobject->AddComponent(new dae::ColliderComponent(pEnemyGameobject));

	//uint16 mask = 0;
	//mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
	//mask |= static_cast<uint16>(dae::PhysicsLayers::layer1);
	//mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);

	pEnemyGameobject->AddComponent(new dae::RigidbodyComponent(pEnemyGameobject, b2_dynamicBody, 1.0f, 1.0f, false));
	auto pEnemyComponent = new EnemyComponent(pEnemyGameobject, nullptr);
	pEnemyGameobject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(m_pScoreDisplay);
	pEnemyComponent->GetSubject()->AddObserver(this);
	
	pEnemyGameobject->SetSerializable(false);
	pEnemyGameobject->SetEnabled(false);
	m_SpawnedObjects.push_back(pEnemyGameobject);

	return pEnemyGameobject;
}
