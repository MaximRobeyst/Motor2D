#include "MiniginPCH.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"
#include "Renderer.h"
#include "RigidbodyComponent.h"

dae::Creator<dae::Component, dae::RigidbodyComponent> s_TranformComponentCreate{};

dae::RigidbodyComponent::RigidbodyComponent(dae::GameObject* pGameobject, b2BodyType bodyType, float density, float friction, bool IsSensor)
	: Component(pGameobject)
	//, m_pTransformComponent{ pGameobject->GetComponent<TransformComponent>() }
	//, m_pWorld { SceneManager::GetInstance().GetScene(1)->GetPhysicsWorld() }
	, m_Density{density}
	, m_Friction{friction}
	, m_Bodydef{bodyType}
	, m_IsSensor{IsSensor}
{
	//m_pColliderComponent = pGameobject->GetComponent<dae::ColliderComponent>();
	//
	//// Create body this is done with the collider
	//b2BodyDef bodyDef;
	//bodyDef.type = bodyType;
	//bodyDef.fixedRotation = true;
	//bodyDef.allowSleep = false;
	//bodyDef.position.Set(m_pTransformComponent->GetPosition().x, m_pTransformComponent->GetPosition().y);
	//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	//m_pBody = m_pWorld->CreateBody(&bodyDef);
	//
	//// Create the fxiture
	//b2FixtureDef fixtureDef;
	//fixtureDef.shape = m_pColliderComponent->GetDynamicBox();
	//fixtureDef.density = m_Density;
	//fixtureDef.friction = m_Friction;
	//fixtureDef.isSensor = IsSensor;
	//
	//m_pBody->CreateFixture(&fixtureDef);
}

dae::RigidbodyComponent::~RigidbodyComponent()
{
	m_OnEnterFunction = nullptr;
	m_OnExitFunction = nullptr;

	m_pWorld->DestroyBody(m_pBody);
}

void dae::RigidbodyComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();
	m_pWorld = m_pGameObject->GetScene()->GetPhysicsWorld();

	m_pColliderComponent = m_pGameObject->GetComponent<dae::ColliderComponent>();

	// Create body this is done with the collider
	b2BodyDef bodyDef;
	bodyDef.type = m_Bodydef;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	bodyDef.position.Set(m_pTransformComponent->GetPosition().x, m_pTransformComponent->GetPosition().y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	m_pBody = m_pWorld->CreateBody(&bodyDef);

	// Create the fxiture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_pColliderComponent->GetDynamicBox();
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;
	fixtureDef.isSensor = m_IsSensor;

	m_pBody->CreateFixture(&fixtureDef);
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

	//auto& pos = m_pTransformComponent->GetPosition();
	//m_pBody->SetTransform(b2Vec2{ pos.x, pos.y }, 0.0f);
}

void dae::RigidbodyComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("Density");
	writer.Double(static_cast<double>(m_Density));
	writer.Key("Friction");
	writer.Double(static_cast<double>(m_Friction));
	writer.Key("IsSensor");
	writer.Bool(m_IsSensor);
	writer.Key("BodyDef");
	writer.Int(static_cast<int>(m_Bodydef));
	writer.EndObject();
}

void dae::RigidbodyComponent::Deserialize(GameObject* pGameObject, rapidjson::Value&value)
{
	m_pGameObject = pGameObject;

	m_Density = static_cast<float>(value["Density"].GetDouble());
	m_Friction = static_cast<float>(value["Friction"].GetDouble());
	m_IsSensor = value["IsSensor"].GetBool();
	m_Bodydef = static_cast<b2BodyType>(value["BodyDef"].GetInt());


}

void dae::RigidbodyComponent::OnBeginContact(dae::RigidbodyComponent* pTriggeredBody, RigidbodyComponent* pOtherBody, b2Contact* pContact)
{
	if (m_OnEnterFunction == nullptr && pOtherBody->m_OnEnterFunction != nullptr)
	{
		pOtherBody->OnBeginContact(pOtherBody, pTriggeredBody, pContact);
	}
	if (m_OnEnterFunction == nullptr) return;

	m_OnEnterFunction(pTriggeredBody, pOtherBody, pContact);
}

void dae::RigidbodyComponent::OnEndContact(dae::RigidbodyComponent* pTriggeredBody, RigidbodyComponent* pOtherBody, b2Contact* pContact)
{
	if (m_OnExitFunction == nullptr && pOtherBody->m_OnExitFunction != nullptr)
	{
		pOtherBody->OnEndContact(pOtherBody, pTriggeredBody, pContact);
	}
	if (m_OnExitFunction == nullptr) return;

	m_OnExitFunction(pTriggeredBody, pOtherBody, pContact);
}

void dae::RigidbodyComponent::SetOnEnterFunction(std::function<void(RigidbodyComponent* ,RigidbodyComponent*, b2Contact*)> newOnEnterFunction)
{
	m_OnEnterFunction = newOnEnterFunction;
}

void dae::RigidbodyComponent::SetOnExitFunction(std::function<void(RigidbodyComponent* ,RigidbodyComponent*, b2Contact*)> newOnExitFunction)
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

void dae::RigidbodyComponent::ChangeShape(b2Shape* pShape)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = pShape;
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;
	fixtureDef.isSensor = m_IsSensor;

	m_pBody->CreateFixture(&fixtureDef);
}
