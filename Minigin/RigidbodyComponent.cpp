#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"

dae::RigidbodyComponent::RigidbodyComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
	, m_pTransformComponent{ pGameobject->GetComponent<TransformComponent>() }
	//, m_pColliderComponent{  }
	, m_pWorld { SceneManager::GetInstance().GetScene(0)->GetPhysicsWorld() }
{
	m_pColliderComponent = pGameobject->GetComponent<ColliderComponent>();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);

	m_pBody = m_pWorld->CreateBody(&bodyDef);
}

void dae::RigidbodyComponent::Update()
{
}

void dae::RigidbodyComponent::Render() const
{
}
