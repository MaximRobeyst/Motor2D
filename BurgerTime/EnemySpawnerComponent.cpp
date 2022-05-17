#include "EnemySpawnerComponent.h"
#include "MrHotDogComponent.h"
#include "Event.h"

dae::Creator<dae::Component, EnemySpawnerComponent> g_EnemySpawnerComponentCreator;

EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
{
}

EnemySpawnerComponent::~EnemySpawnerComponent()
{
}

void EnemySpawnerComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	if (action == Event::Enemy_Died)
		m_Count--;
}

void EnemySpawnerComponent::Update()
{
	if (m_Count >= m_MaxCount)
		SpawnEnemyAtLocation(m_SpawnPoints[m_Count]);
}

void EnemySpawnerComponent::SpawnEnemyAtLocation(glm::vec3 position)
{
	// Spawn Enemy
}
