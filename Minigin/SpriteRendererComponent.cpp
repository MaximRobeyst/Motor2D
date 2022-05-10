#include "MiniginPCH.h"
#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture2D.h"

dae::SpriteRendererComponent::SpriteRendererComponent(GameObject* pGameObject,const std::string& spritePath, SDL_FRect sampleRect)
	: Component{pGameObject}
	, m_pTexture{ResourceManager::GetInstance().LoadTexture(spritePath)}
	, m_SampleRectangle{sampleRect}
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
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

void dae::SpriteRendererComponent::Render() const
{
	const auto& pos{ m_pTransformComponent->GetPosition() };
	const auto& scale{ m_pTransformComponent->GetScale() };
	const auto& textureSize{ m_pTexture->GetSize() };

	if (IsSampleRectEmpty())
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, textureSize.x * scale.x, textureSize.y * scale.y);
	else
		Renderer::GetInstance().RenderTexture(*m_pTexture, m_SampleRectangle, pos.x, pos.y, m_SampleRectangle.w * abs(scale.x), m_SampleRectangle.h * abs(scale.y), scale.x < 0);
}

void dae::SpriteRendererComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

bool dae::SpriteRendererComponent::IsSampleRectEmpty() const
{
	return m_SampleRectangle.x == 0.0f && m_SampleRectangle.y == 0.0f && m_SampleRectangle.w == 0.0f && m_SampleRectangle.h == 0.0f;
}
