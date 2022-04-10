#include "MiniginPCH.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"
#include "Renderer.h"
#include "RigidbodyComponent.h"

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
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	m_pBody = m_pWorld->CreateBody(&bodyDef);
	m_pBody->SetFixedRotation(true);

	// Create the fxiture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pColliderComponent->GetDynamicBox();
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;
	fixtureDef.isSensor = true;

	m_pBody->CreateFixture(&fixtureDef);
}

dae::RigidbodyComponent::~RigidbodyComponent()
{
	m_pWorld->DestroyBody(m_pBody);
}

void dae::RigidbodyComponent::Update()
{
}

void dae::RigidbodyComponent::Render() const
{
	//auto pos = m_pBody->GetPosition();
	//auto size = m_pColliderComponent->GetSize();
	//
	//Renderer::GetInstance().RenderBox(pos.x, pos.y, size.x, size.y);

	auto& pos = m_pTransformComponent->GetPosition();
	m_pBody->SetTransform(b2Vec2{ pos.x, pos.y }, 0.0f);
}

void dae::RigidbodyComponent::OnBeginContact(RigidbodyComponent* pOtherBody, b2Contact* pContact)
{
	if (m_OnEnterFunction == nullptr && pOtherBody->m_OnEnterFunction != nullptr)
	{
		pOtherBody->OnBeginContact(this, pContact);
	}
	if (m_OnEnterFunction == nullptr) return;

	m_OnEnterFunction(pOtherBody, pContact);
}

void dae::RigidbodyComponent::OnEndContact(RigidbodyComponent* pOtherBody, b2Contact* pContact)
{
	m_OnExitFunction(pOtherBody, pContact);
}

void dae::RigidbodyComponent::SetOnEnterFunction(std::function<void(RigidbodyComponent*, b2Contact*)> newOnEnterFunction)
{
	m_OnEnterFunction = newOnEnterFunction;
}

void dae::RigidbodyComponent::SetOnExitFunction(std::function<void(RigidbodyComponent*, b2Contact*)> newOnExitFunction)
{
	m_OnExitFunction = newOnExitFunction;
}

b2Vec2 dae::RigidbodyComponent::GetPosition() const
{
	return m_pBody->GetPosition();
}

b2Body* dae::RigidbodyComponent::GetBody() const
{
	return m_pBody;
}

dae::GameObject* dae::RigidbodyComponent::GetGameobject() const
{
	return m_pGameObject;
}
