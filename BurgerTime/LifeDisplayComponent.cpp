#include "MiniginPCH.h"
#include "LifeDisplayComponent.h"
#include "Event.h"
#include "LifeComponent.h"

#include <GameObject.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <Scene.h>

LifeDisplayComponent::LifeDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText)
	: dae::Component{pGameObject}
	, m_ExtraDisplayText{extraDisplayText}
{
	//m_pTextComponent = pGameObject->GetComponent<dae::TextComponent>();
	//m_pTextComponent->SetText(extraDisplayText + std::to_string(number));

	auto scene = dae::SceneManager::GetInstance().GetScene(0);

	m_pLifeSprites.reserve(number);
	for (int i = 0; i < number; ++i)
	{
		auto go = new dae::GameObject();
		go->SetParent(m_pGameObject);
		go->AddComponent(new dae::TransformComponent(go, glm::vec3{ 0.0f, -(i * 20.f), 0.0f}, glm::vec3{ 2.f }));
		go->AddComponent(new dae::SpriteRendererComponent(go, "BurgerTime_SpriteSheet.png", SDL_FRect(200.f, 0, 8, 8 )));

		scene->AddGameObject(go);

		m_pLifeSprites.push_back(go);

	}
}

void LifeDisplayComponent::Render() const
{
}

void LifeDisplayComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	switch (action)
	{
	case Event::Player_Died:
		 ChangeText(gameObject.GetComponent<LifeComponent>()->GetLives());
		 dae::SceneManager::GetInstance().GetScene(0)->RemoveGameObject(m_pLifeSprites[m_pLifeSprites.size() - 1]);
		 m_pLifeSprites.pop_back();

	default:
		break;
	}
}

void LifeDisplayComponent::ChangeText(int /*number*/)
{
	//m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}
