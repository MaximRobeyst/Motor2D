#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"

dae::RigidbodyComponent::RigidbodyComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
	, m_pTransformComponent{ pGameobject->GetComponent<TransformComponent>() }
	//, m_pColliderComponent{  }
	, m_pWorld { SceneManager::GetInstance().GetScene(0)->GetPhysicsWorld() }
{
	m_pColliderComponent = pGameobject->GetComponent<dae::ColliderComponent>();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);


	m_pBody = m_pWorld->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_pBody->CreateFixture(&fixtureDef);

}

void dae::RigidbodyComponent::Update()
{
	auto transform = m_pBody->GetTransform();
	m_pTransformComponent->SetPosition(transform.p.x, transform.p.y, 0.0f);
}

void dae::RigidbodyComponent::Render() const
{
}
