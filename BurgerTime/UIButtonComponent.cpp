#include "UIButtonComponent.h"

#include <GameObject.h>
#include <TextObject.h>
#include <InputManager.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <Renderer.h>
#include <Keyboard.h>
#include <iostream>

UIButtonComponent::UIButtonComponent(dae::GameObject* pGameobject, glm::vec4 color)
	: Component(pGameobject)
	, m_NewColor{}
{
}

void UIButtonComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pSpriteComponent = m_pGameObject->GetComponent<dae::SpriteRendererComponent>();
	m_pTextComponent = m_pGameObject->GetComponent<dae::TextComponent>();

	glm::vec3 halfWidth = glm::vec3
	(m_pSpriteComponent->GetSampleRectangle().w / 2, m_pSpriteComponent->GetSampleRectangle().h / 2, 0);

	m_BoundsMin = m_pTransformComponent->GetPosition() - halfWidth;
	m_BoundsMax = m_pTransformComponent->GetPosition() + halfWidth;
	
}

void UIButtonComponent::Update()
{
	auto mousePosition = dae::InputManager::GetInstance().GetMousePosition();

	if (OverLapping(mousePosition))
	{
		m_pTextComponent->SetText("Hovering");
		m_pTextComponent->SetColor(SDL_Color{ 255,0,0,255 });

		if (dae::InputManager::GetInstance().GetKeyboard()->IsPressed((int)SDLK_e))
			m_OnClickFunction();
	}
	else
	{
		m_pTextComponent->SetText("not Hovering");
		m_pTextComponent->SetColor(SDL_Color{ 255,255,255,255 });
	}
}

void UIButtonComponent::Render() const
{
	dae::Renderer::GetInstance().RenderBox(m_BoundsMin.x, m_BoundsMin.y,
		m_BoundsMax.x - m_BoundsMin.x, m_BoundsMax.y - m_BoundsMin.y);
}

void UIButtonComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void UIButtonComponent::SetOnClickFunction(std::function<void()> function)
{
	m_OnClickFunction = function;
}

bool UIButtonComponent::OverLapping(const glm::vec2& mousePosition) const
{
	return mousePosition.x > m_BoundsMin.x && mousePosition.x < m_BoundsMax.x 
		&& mousePosition.y > m_BoundsMin.y && mousePosition.y < m_BoundsMax.y;
}
