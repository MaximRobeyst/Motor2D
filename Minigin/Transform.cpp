#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include <imgui.h>

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position, glm::vec3 scale)
	: Component{pGameobject}
	, m_Transform{ position, glm::vec4{}, scale }
{
	m_pParentComponent = pGameobject->GetComponent<dae::TransformComponent>();
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Transform.position.x = x;
	m_Transform.position.y = y;
	m_Transform.position.z = z;

	if (m_pParentComponent != nullptr)
		m_Transform.position += m_pParentComponent->GetPosition();
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Transform.position = position;
}

void dae::TransformComponent::Move(const glm::vec3& moveVector)
{
	m_Transform.position += moveVector;
}

void dae::TransformComponent::SetScale(float x, float y, float z)
{
	m_Transform.scale.x = x;
	m_Transform.scale.y = y;
	m_Transform.scale.z = z;
}

void dae::TransformComponent::SetScale(const glm::vec3& scale)
{
	m_Transform.scale = scale;
}

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::Render() const
{
}

dae::TransformComponent::Transform& dae::TransformComponent::GetTransform()
{
	return m_Transform;
}
