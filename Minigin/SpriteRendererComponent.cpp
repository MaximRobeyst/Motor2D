#include "MiniginPCH.h"
#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture2D.h"

dae::Creator<dae::Component, dae::SpriteRendererComponent> s_TranformComponentCreate{};

dae::SpriteRendererComponent::SpriteRendererComponent(GameObject* pGameObject,const std::string& spritePath, SDL_FRect sampleRect)
	: Component{pGameObject}
	, m_Path{ spritePath }
	, m_pTexture{ResourceManager::GetInstance().LoadTexture(spritePath)}
	, m_SampleRectangle{sampleRect}
{
}

void dae::SpriteRendererComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_pTexture = texture;
}
const std::shared_ptr<dae::Texture2D>& dae::SpriteRendererComponent::GetTexture() const
{
	return m_pTexture;
}
void dae::SpriteRendererComponent::SetSampleRectangle(const SDL_FRect& sampleRect)
{
	m_SampleRectangle = sampleRect;
}

SDL_FRect dae::SpriteRendererComponent::GetSampleRectangle()
{
	if (IsSampleRectEmpty())
	{
		return SDL_FRect{ 0,0, m_pTexture->GetSize().x, m_pTexture->GetSize().y };
	}


	return m_SampleRectangle;
}

void dae::SpriteRendererComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
}

void dae::SpriteRendererComponent::Render() const
{
	const auto& pos{ m_pTransformComponent->GetPosition() };
	const auto& scale{ m_pTransformComponent->GetScale() };
	const auto& rotation{ static_cast<double>(m_pTransformComponent->GetRotation()) };
	const auto& textureSize{ m_pTexture->GetSize() };

	if (IsSampleRectEmpty())
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, textureSize.x * scale.x, textureSize.y * scale.y, rotation);
	else
		Renderer::GetInstance().RenderTexture(*m_pTexture, m_SampleRectangle, pos.x, pos.y, m_SampleRectangle.w * abs(scale.x), m_SampleRectangle.h * abs(scale.y), rotation, scale.x < 0);
}

void dae::SpriteRendererComponent::RenderGUI()
{
	float rect[4] = { m_SampleRectangle.x, m_SampleRectangle.y, m_SampleRectangle.w, m_SampleRectangle.h };
	if (ImGui::InputFloat4("Sample rect", rect))
		m_SampleRectangle = SDL_FRect{ rect[0], rect[1], rect[2], rect[3] };
}

void dae::SpriteRendererComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("Path");
	writer.String(m_Path.c_str());
	writer.Key("SampleRect");
	writer.StartObject();
	writer.Key("x");
	writer.Double(static_cast<double>(m_SampleRectangle.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_SampleRectangle.y));
	writer.Key("w");
	writer.Double(static_cast<double>(m_SampleRectangle.w));
	writer.Key("h");
	writer.Double(static_cast<double>(m_SampleRectangle.h));
	writer.EndObject();
	writer.EndObject();
}

void dae::SpriteRendererComponent::Deserialize(GameObject* pGameObject, rapidjson::Value& value)
{
	m_pGameObject = pGameObject;
	m_Path = value["Path"].GetString();
	m_pTexture = ResourceManager::GetInstance().LoadTexture(m_Path);
	m_SampleRectangle = SDL_FRect{
		static_cast<float>(value["SampleRect"]["x"].GetDouble()),
		static_cast<float>(value["SampleRect"]["y"].GetDouble()),
		static_cast<float>(value["SampleRect"]["w"].GetDouble()),
		static_cast<float>(value["SampleRect"]["h"].GetDouble())
	};
}

bool dae::SpriteRendererComponent::IsSampleRectEmpty() const
{
	return m_SampleRectangle.x == 0.0f && m_SampleRectangle.y == 0.0f && m_SampleRectangle.w == 0.0f && m_SampleRectangle.h == 0.0f;
}
