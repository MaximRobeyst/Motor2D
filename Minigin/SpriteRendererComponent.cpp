#include "MiniginPCH.h"
#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::SpriteRendererComponent::SpriteRendererComponent(dae::GameObject* pGameObject,const std::string& spritePath)
	: Component{pGameObject}
	, m_pTexture{ResourceManager::GetInstance().LoadTexture(spritePath)}
{
}

void dae::SpriteRendererComponent::Update()
{
}

void dae::SpriteRendererComponent::Render() const
{
	const auto& pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
