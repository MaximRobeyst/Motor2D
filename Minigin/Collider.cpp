#include "MiniginPCH.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "SpriteRendererComponent.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "Scene.h"

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject)
	: Component(pGameObject)
	, m_DynamicBox{new b2PolygonShape()}
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();
	auto pTextureComponent = pGameObject->GetComponent<SpriteRendererComponent>();
	if (pTextureComponent != nullptr)
	{
		m_Width = pTextureComponent->GetTexture()->GetSize().x * m_pTransform->GetScale().x;
		m_Height = pTextureComponent->GetTexture()->GetSize().y * m_pTransform->GetScale().y;
	}

	m_Center.x = (m_Width / 2);
	m_Center.y = (m_Height / 2);
	m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2, b2Vec2{ m_Center.x, m_Center.y }, 0.0f);
}

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject, float width, float height, glm::vec2 center)
	: Component(pGameObject)
	, m_Center{center}
	, m_DynamicBox{ new b2PolygonShape() }
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();
	m_Width = (width * m_pTransform->GetScale().x);
	m_Height = (height * m_pTransform->GetScale().y);

	m_Center.x = (m_Width / 2);
	m_Center.y = (m_Height / 2);

	m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2 , b2Vec2{m_Center.x, m_Center.y}, 0.0f);
}

dae::ColliderComponent::~ColliderComponent()
{
	delete m_DynamicBox;
}

void dae::ColliderComponent::Update()
{
}

void dae::ColliderComponent::Render() const
{
	auto pos = m_pTransform->GetPosition();
	Renderer::GetInstance().RenderBox((pos.x + m_Center.x) - (m_Width / 2), (pos.y + m_Center.y) - (m_Height / 2), m_Width, m_Height);
}

#ifdef _DEBUG
void dae::ColliderComponent::RenderGUI()
{
	ImGui::Text("Collider");
	ImGui::DragFloat("Width", &m_Width, 1.0f, 1.0f, 100.0f);
	ImGui::DragFloat("Height", &m_Height, 1.0f, 1.0f, 100.0f);
	float center[2]{ m_Center.x, m_Center.y };
	ImGui::DragFloat2("Center", center);
	m_Center.x = center[0];
	m_Center.y = center[1];
}
#endif // _DEBUG

b2PolygonShape* dae::ColliderComponent::GetDynamicBox() const
{
	return m_DynamicBox;
}