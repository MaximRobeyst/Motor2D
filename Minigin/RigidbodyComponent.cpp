#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"

dae::RigidbodyComponent::RigidbodyComponent(dae::GameObject* pGameobject, b2BodyType bodyType, float density, float friction)
	: Component(pGameobject)
	, m_pTransformComponent{ pGameobject->GetComponent<TransformComponent>() }
	, m_pWorld { SceneManager::GetInstance().GetScene(0)->GetPhysicsWorld() }
	, m_Density{density}
	, m_Friction{friction}
{
	m_pColliderComponent = pGameobject->GetComponent<dae::ColliderComponent>();

	// Create body this is done with the collider
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(m_pTransformComponent->GetPosition().x, m_pTransformComponent->GetPosition().y);
	m_pBody = m_pWorld->CreateBody(&bodyDef);
	m_pBody->SetFixedRotation(true);

	// Create the fxiture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pColliderComponent->GetDynamicBox();
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;

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

void dae::RigidbodyComponent::SetVelocity(glm::vec2 velocity)
{
	m_pBody->SetLinearVelocity( b2Vec2{velocity.x, velocity.y} );
}