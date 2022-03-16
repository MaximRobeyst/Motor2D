#include "MiniginPCH.h"
#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture2D.h"

dae::SpriteRendererComponent::SpriteRendererComponent(GameObject* pGameObject,const std::string& spritePath)
	: Component{pGameObject}
	, m_pTexture{ResourceManager::GetInstance().LoadTexture(spritePath)}
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
}

void dae::SpriteRendererComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_pTexture = texture;
}
void dae::SpriteRendererComponent::Render() const
{
	const auto& pos{ m_pTransformComponent->GetPosition() };
	const auto& scale{ m_pTransformComponent->GetScale() };
	const auto& textureSize{ m_pTexture->GetSize() };
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, textureSize.x * scale.x, textureSize.y* scale.y );
}
