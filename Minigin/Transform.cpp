#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include <imgui.h>

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position, glm::vec3 scale)
	: Component{pGameobject}
	, m_Position{position}
	, m_Scale{scale}
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

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void dae::TransformComponent::Move(const glm::vec3& moveVector)
{
	m_Position += moveVector;
}

void dae::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void dae::TransformComponent::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::Render() const
{
}
