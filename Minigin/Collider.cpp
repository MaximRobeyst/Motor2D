#include "MiniginPCH.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "SpriteRendererComponent.h"
#include "Texture2D.h"

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject)
	: Component(pGameObject)
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();
	auto pTextureComponent = pGameObject->GetComponent<SpriteRendererComponent>();
	if (pTextureComponent != nullptr)
	{
		m_Width = pTextureComponent->GetTexture()->GetSize().x * m_pTransform->GetScale().x;
		m_Height = pTextureComponent->GetTexture()->GetSize().y * m_pTransform->GetScale().y;
	}
	
}

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject, float width, float height)
	: Component(pGameObject)
	, m_Width{width}
	, m_Height{height}
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();
}

void dae::ColliderComponent::Update()
{
}

void dae::ColliderComponent::Render() const
{
	auto pos = m_pTransform->GetPosition();
	Renderer::GetInstance().RenderBox(pos.x, pos.y, m_Width, m_Height);
}
