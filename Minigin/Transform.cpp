#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position)
	: Component{pGameobject}
	, m_Position{position}
{
	m_pParentComponent = pGameobject->GetComponent<dae::TransformComponent>();
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	if (m_pParentComponent != nullptr)
		m_Position += m_Position;
}

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::Render() const
{
}
