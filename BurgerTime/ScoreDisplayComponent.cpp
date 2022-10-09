#include "ScoreDisplayComponent.h"
#include "Observer.h"
#include "Event.h"
#include "Subject.h"
#include "MrHotDogComponent.h"

#include <GameObject.h>
#include <string>
#include <Factory.h>
#include <Scene.h>

#include "LifeComponent.h"
#include <GameTime.h>
#include <Utils.h>
#include <Renderer.h>
#include <SpriteRendererComponent.h>
#include <Collider.h>
#include <RigidbodyComponent.h>

#include "ProjectileComponent.h"
#include "EnemySpawnerComponent.h"

const dae::Creator<dae::Component, ScoreDisplayComponent> g_ScoredisplayCreator{};

int ScoreDisplayComponent::m_Score{};


ScoreDisplayComponent::ScoreDisplayComponent()
	: m_pSubject{std::make_unique<Subject>()}
{
}

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pGameObject, int /*number*/, const std::string& extraDisplayText)
	: Component{ pGameObject }
	, m_ExtraDisplayText{extraDisplayText}
	, m_pSubject{std::make_unique<Subject>()}
{
}

void ScoreDisplayComponent::Start()
{
	SetId(m_pGameObject->GetId());
	m_pTextComponent = m_pGameObject->GetComponent<dae::TextComponent>();
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(m_Score));

	m_EnemySpawnerComponent = m_pGameObject->GetScene()->FindGameobjectWithTag("Spawner")->GetComponent<EnemySpawnerComponent>();
}

void ScoreDisplayComponent::Update()
{
	m_Timer += GameTime::GetInstance()->GetElapsed();
	m_TimerTillNextSpawn += GameTime::GetInstance()->GetElapsed();

	if (m_Timer >= 1.0f)
	{
		m_Score += 1;
		m_Timer -= 1.0f;
		ChangeText(m_Score);
	}

	if (m_TimerTillNextSpawn >= 10.f)
	{
		SpawnProjectile();
		SpawnEnemy();

		m_TimerTillNextSpawn -= 10.f;
	}
}

void ScoreDisplayComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("extraDisplayText");
	writer.String(m_ExtraDisplayText.c_str());
	writer.Key("Observers");
	writer.StartArray();
	for (int i = 0; i < m_pSubject->GetNrOfObservers(); ++i)
	{
		writer.Int(m_pSubject->GetObserverFromId(i)->GetId());
	}
	writer.EndArray();
	writer.EndObject();
}

void ScoreDisplayComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	
	m_ExtraDisplayText = value["extraDisplayText"].GetString();

	for (auto iter = value["Observers"].Begin(); iter != value["Observers"].End(); ++iter)
	{
		auto pObserver = pGameobject->GetScene()->GetGameobjectFromId(iter->GetInt());

		m_pSubject->AddObserver(pObserver->GetComponent<Observer>());

	}

}

void ScoreDisplayComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
	case Event::Enemy_Died:
	{
		m_Score += 100;
		ChangeText(m_Score);
		break;
	}
	case Event::Second_Survived:
	{
		m_Score += 1;
		break;
	}
	default:
		break;
	}
}

void ScoreDisplayComponent::ChangeText(int number)
{
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}

int ScoreDisplayComponent::GetScore()
{
	return m_Score;
}

void ScoreDisplayComponent::ResetScore()
{
	m_Score = 0;
}

std::unique_ptr<Subject>& ScoreDisplayComponent::GetSubject()
{
	return m_pSubject;
}

void ScoreDisplayComponent::SpawnProjectile()
{
	auto pProjectile = new dae::GameObject("Bullet");
	pProjectile->SetTag("Projectile");

	pProjectile->AddComponent(new dae::TransformComponent(pProjectile, glm::vec3{Random(25.f, dae::Renderer::GetInstance().GetWindowWidth() - 25.f), Random(25.f, dae::Renderer::GetInstance().GetWindowHeight() - 25.f), 0.f}));
	pProjectile->GetTransform()->SetRotation(Random(-static_cast<float>(M_PI), static_cast<float>(M_PI)));
	pProjectile->AddComponent(new dae::SpriteRendererComponent(pProjectile, "Sprites/Arrow.png"));
	pProjectile->AddComponent(new dae::ColliderComponent(pProjectile));


	uint16 mask = 0;
	mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer2);
	mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);
	pProjectile->AddComponent(new dae::RigidbodyComponent(pProjectile, b2_dynamicBody, 1.0f, 1.0f, false, dae::PhysicsLayers::layer1, mask));
	pProjectile->AddComponent(new ProjectileComponent(500.f, true));

	GetGameObject()->GetScene()->AddGameObject(pProjectile);

}

void ScoreDisplayComponent::SpawnEnemy()
{
}
