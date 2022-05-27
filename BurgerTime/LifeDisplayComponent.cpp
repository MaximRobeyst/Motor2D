#include "LifeDisplayComponent.h"
#include "Event.h"
#include "LifeComponent.h"

#include <GameObject.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <Scene.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning (pop)
#include <sdl.h>
#include <GameStateManager.h>

#include "LeaderboardState.h"

LifeDisplayComponent::LifeDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText)
	: dae::Component{pGameObject}
	, m_ExtraDisplayText{extraDisplayText}
	, m_NumberOfLives{number}
{
}

void LifeDisplayComponent::Start()
{
	auto scene = m_pGameObject->GetScene();

	m_pLifeSprites.reserve(m_NumberOfLives);
	for (int i = 0; i < m_NumberOfLives; ++i)
	{
		auto go = new dae::GameObject("Live");
		go->SetParent(m_pGameObject);
		go->AddComponent(new dae::TransformComponent(go, glm::vec3{ 0.0f, -(i * 20.f), 0.0f }, glm::vec3{ 2.f }));
		go->AddComponent(new dae::SpriteRendererComponent(go, "BurgerTime_SpriteSheet.png", SDL_FRect{ 200.f, 0.f, 8.f, 8.f }));

		scene->AddGameObject(go);

		m_pLifeSprites.push_back(go);

	}
}

void LifeDisplayComponent::Render() const
{
}

void LifeDisplayComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void LifeDisplayComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	switch (action)
	{
	case Event::Player_Died:
		 ChangeText(gameObject.GetComponent<LifeComponent>()->GetLives());

		 if (gameObject.GetComponent<LifeComponent>()->GetLives() <= 0)
		 {
			 GameStateManager::GetInstance().SwitchGameState(new LeaderboardState(100));
			 return;
		 }

		 m_pGameObject->GetScene()->RemoveGameObject(m_pLifeSprites[m_pLifeSprites.size() - 1]);
		 m_pLifeSprites.pop_back();
	default:
		break;
	}
}

void LifeDisplayComponent::ChangeText(int /*number*/)
{
	//m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}
