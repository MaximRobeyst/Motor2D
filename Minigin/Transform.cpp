#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include <imgui.h>

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position, glm::vec2 scale)
	: Component{pGameobject}
	, m_Transform{ position, position, 0.0f, scale }
{
	if(pGameobject->GetParent() != nullptr)
		m_pParentComponent = pGameobject->GetParent()->GetComponent<dae::TransformComponent>();
	if (m_pParentComponent != nullptr)
		m_Transform.worldPosition += m_pParentComponent->GetPosition();
}

const glm::vec3& dae::TransformComponent::GetPosition() const 
{
	return m_Transform.worldPosition; 
}

void dae::TransformComponent::SetPosition(const float x, const float y)
{
	m_Dirty = true;

	m_Transform.worldPosition.x = x;
	m_Transform.worldPosition.y = y;

	//if (m_pParentComponent != nullptr)
	//	m_Transform.worldPosition += m_pParentComponent->GetPosition();
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Dirty = true;
	m_Transform.worldPosition = position;
}

void dae::TransformComponent::Move(const glm::vec3& moveVector)
{
	m_Dirty = true;
	m_Transform.worldPosition += moveVector;
}

void dae::TransformComponent::SetScale(float x, float y)
{
	m_Dirty = true;
	m_Transform.scale.x = x;
	m_Transform.scale.y = y;
}

void dae::TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Dirty = true;
	m_Transform.scale = scale;
}

void dae::TransformComponent::SetDirty()
{
	m_Dirty = true;
}

bool dae::TransformComponent::IsDirty() const
{
	return m_Dirty;
}

void dae::TransformComponent::Start()
{
	m_pRigidbodyComponent = m_pGameObject->GetComponent<RigidbodyComponent>();
}

void dae::TransformComponent::Update()
{
	if (m_pRigidbodyComponent)
	{
		if (m_Dirty)
		{
			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ m_Transform.worldPosition.x, m_Transform.worldPosition.y }, m_Transform.rotation);
			m_Dirty = false;
		}
		else
		{
			auto pos = m_pRigidbodyComponent->GetBody()->GetPosition();;
			m_Transform.worldPosition = glm::vec3{ pos.x, pos.y , 0.f};
		}
	}
}

void dae::TransformComponent::Render() const
{
}

dae::TransformComponent::Transform& dae::TransformComponent::GetTransform()
{
	m_Dirty = true;
	return m_Transform;
}

const dae::TransformComponent::Transform& dae::TransformComponent::GetTransformConst() const
{
	return m_Transform;
}
