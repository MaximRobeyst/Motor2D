#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "SpriteRendererComponent.h"

#include "ResourceManager.h"

dae::Creator<dae::Component, dae::TextComponent> g_TextComponentCreator{};

dae::TextComponent::TextComponent(GameObject* pGameObject, const std::string& text, Font* font, const SDL_Color& color)
	: dae::Component{ pGameObject }
	, m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_Font{ font }
	, m_pTextTexture{ nullptr }
	, m_Color{color}
{
}

dae::TextComponent::~TextComponent()
{
	delete m_pTextTexture;
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
		if (m_pTextTexture != nullptr)
			delete m_pTextTexture;

		m_pTextTexture = new Texture2D(texture, "");
		m_NeedsUpdate = false;

		auto comp = m_pGameObject->GetComponent<dae::SpriteRendererComponent>();
		comp->SetTexture(m_pTextTexture);
	}
}

void dae::TextComponent::Render() const
{
	if (m_pTextTexture != nullptr)
	{
		//const auto& pos = m_pGameObject->GetComponent<Transform>()->GetPosition();
		//Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());
	writer.Key("Text");
	writer.String(m_Text.c_str());
	writer.Key("Color");
	writer.StartObject();
	writer.Key("r");
	writer.Int((m_Color.r));
	writer.Key("g");
	writer.Int((m_Color.g));
	writer.Key("b");
	writer.Int((m_Color.b));
	writer.Key("a");
	writer.Int((m_Color.a));
	writer.EndObject();
	writer.EndObject();

}

void dae::TextComponent::Deserialize(GameObject* pGameobect, rapidjson::Value& value)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	m_pGameObject = pGameobect;
	m_Text = value["Text"].GetString();
	m_Font = font;
	m_Color =
		SDL_Color{
			static_cast<uint8_t>(value["Color"]["r"].GetInt()),
			static_cast<uint8_t>(value["Color"]["g"].GetInt()),
			static_cast<uint8_t>(value["Color"]["b"].GetInt()),
			static_cast<uint8_t>(value["Color"]["a"].GetInt())
	};
	
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	if (m_Text == text) return;

	m_Text = text;
	m_NeedsUpdate = true;
}

std::string dae::TextComponent::GetText() const
{
	return m_Text;
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

