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
	m_pBody->SetLinearVelocity(m_RigidBody.velocity);
	auto transform = m_pBody->GetTransform();
	m_pTransformComponent->SetPosition(transform.p.x, transform.p.y, 0.0f);

	m_RigidBody.velocity = m_pBody->GetLinearVelocity();
}

void dae::RigidbodyComponent::Render() const
{
}

void dae::RigidbodyComponent::SetVelocity(const glm::vec2& velocity)
{
	m_RigidBody.velocity = b2Vec2{ velocity.x, velocity.y } + m_pBody->GetLinearVelocity();

	m_pBody->SetLinearVelocity( m_RigidBody.velocity );
}

void dae::RigidbodyComponent::AddForce(const glm::vec2& force)
{
	m_pBody->ApplyForce(b2Vec2{force.x, force.y}, m_pBody->GetPosition(), true);
}

dae::RigidbodyComponent::RigidBody& dae::RigidbodyComponent::GetRigidbody()
{
	return m_RigidBody;
}
