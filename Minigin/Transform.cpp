#include "MiniginPCH.h"
#include "Transform.h"

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position)
	: Component{pGameobject}
	, m_Position{position}
{
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::Render() const
{
}
