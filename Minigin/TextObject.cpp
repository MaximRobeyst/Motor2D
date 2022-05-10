#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "SpriteRendererComponent.h"

dae::TextComponent::TextComponent(GameObject* pGameObject, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color)
	: dae::Component{ pGameObject }
	, m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_Font{ font }
	, m_TextTexture{ nullptr }
	, m_Color{color}
{
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		//const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;

		auto comp = m_pGameObject->GetComponent<dae::SpriteRendererComponent>();
		comp->SetTexture(m_TextTexture);
	}
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		//const auto& pos = m_pGameObject->GetComponent<Transform>()->GetPosition();
		//Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
	//m_Transform.SetPosition(x, y, 0.0f);
	m_pGameObject->GetComponent<TransformComponent>()->SetPosition(x, y);
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}

