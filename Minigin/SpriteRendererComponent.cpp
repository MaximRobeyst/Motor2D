#include "MiniginPCH.h"
#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"

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
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
