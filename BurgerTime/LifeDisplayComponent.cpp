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
#include "ScoreDisplayComponent.h"

const dae::Creator<dae::Component, LifeDisplayComponent> g_LifeDisplayComponent{};

LifeDisplayComponent::LifeDisplayComponent(dae::GameObject* pGameObject, int /*number*/, const std::string& extraDisplayText)
	: dae::Component{pGameObject}
	, m_ExtraDisplayText{extraDisplayText}
{
}

void LifeDisplayComponent::Start()
{
	SetId(m_pGameObject->GetId());
	m_pTextComponent = m_pGameObject->GetComponent<dae::TextComponent>();
	m_pLifeComponent = m_pGameObject->GetScene()->FindGameobjectWithTag("Player")->GetComponent<LifeComponent>();
	ChangeText(m_pLifeComponent->GetLives());
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
			 GameStateManager::GetInstance().SwitchGameState(new LeaderboardState(ScoreDisplayComponent::GetScore()));
			 return;
		 }
	default:
		break;
	}
}

void LifeDisplayComponent::ChangeText(int number)
{
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}
