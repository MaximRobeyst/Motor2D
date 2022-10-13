#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include <imgui.h>

#include "RapidjsonHelpers.h"
#include "imgui_helpers.h"

dae::Creator<dae::Component, dae::TransformComponent> s_TranformComponentCreate{};

dae::TransformComponent::TransformComponent()
	: Component{}
	, m_WorldTransform{ glm::vec3{}, 0.0f, glm::vec2{1.0f} }
{

}

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position, glm::vec2 scale)
	: Component{pGameobject}
	, m_WorldTransform{ position, 0.0f, scale }
{
	if(pGameobject->GetParent() != nullptr)
		m_pParentComponent = pGameobject->GetParent()->GetComponent<dae::TransformComponent>();
	//if (m_pParentComponent != nullptr)
	//	m_Transform.worldPosition += m_pParentComponent->GetPosition();
}

void dae::TransformComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	rapidjson::SerializeValue(writer, "Name", std::string(typeid(*this).name()));
	rapidjson::SerializeValue(writer, "Position", m_WorldTransform.position);
	rapidjson::SerializeValue(writer, "Rotation", m_WorldTransform.rotation);
	rapidjson::SerializeValue(writer, "Scale", m_WorldTransform.scale);
	writer.EndObject();
}

void dae::TransformComponent::Deserialize(GameObject* pGameObject, rapidjson::Value& value)
{
	m_pGameObject = pGameObject;

	rapidjson::DeserializeValue(value, "Position", m_WorldTransform.position);
	rapidjson::DeserializeValue(value, "Rotation", m_WorldTransform.rotation);
	rapidjson::DeserializeValue(value, "Scale", m_WorldTransform.scale);
}

void dae::TransformComponent::SetParent(dae::GameObject* pParent, bool keepPos)
{
	if (m_pGameObject->GetParent() != pParent) { m_pGameObject->SetParent(pParent); return; }


	if (keepPos)
	{
		m_WorldTransform.position = GetPosition();
		m_WorldTransform.rotation = GetRotation();
		m_WorldTransform.scale = GetScale();
	}

	if (pParent != nullptr)
		m_pParentComponent = pParent->GetTransform();
	else
		m_pParentComponent = nullptr;


}

const glm::vec3 dae::TransformComponent::GetPosition() const
{
	auto pos = m_WorldTransform.position;
	if (m_pParentComponent) pos += m_pParentComponent->GetPosition();

	return pos; 
}

void dae::TransformComponent::SetPosition(const float x, const float y)
{
	m_Dirty = true;

	m_WorldTransform.position.x = x;
	m_WorldTransform.position.y = y;

	//if (m_pParentComponent != nullptr)
	//	m_Transform.worldPosition += m_pParentComponent->GetPosition();
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Dirty = true;
	m_WorldTransform.position = position;
}

void dae::TransformComponent::SetPosition(const b2Vec2& position)
{
	m_Dirty = true;
	m_WorldTransform.position.x = position.x;
	m_WorldTransform.position.y = position.y;
}

void dae::TransformComponent::Move(const glm::vec3& moveVector)
{
	m_Dirty = true;
	m_WorldTransform.position += moveVector;
}

glm::vec2 dae::TransformComponent::GetForward() const
{
	return glm::vec2{cosf(GetRotation()), sinf(GetRotation())};
}

glm::vec2 dae::TransformComponent::GetRight() const
{
	return glm::vec2();
}

const glm::vec2 dae::TransformComponent::GetScale() const
{ 
	auto scale = m_WorldTransform.scale;
	if (m_pParentComponent) scale *= m_pParentComponent->m_WorldTransform.scale;
	return scale;
}

void dae::TransformComponent::SetScale(float x, float y)
{
	m_Dirty = true;
	m_WorldTransform.scale.x = x;
	m_WorldTransform.scale.y = y;
}

void dae::TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Dirty = true;
	m_WorldTransform.scale = scale;
}

const float dae::TransformComponent::GetRotation() const
{
	auto rotation = m_WorldTransform.rotation;
	if (m_pParentComponent) rotation += m_pParentComponent->GetRotation();
	return rotation;
}

void dae::TransformComponent::SetRotation(float rot)
{
	m_Dirty = true;
	m_WorldTransform.rotation = rot;
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
		if (m_Dirty && GetRotation() != 0)
		{
			auto body = m_pRigidbodyComponent->GetBody();
			auto position = body->GetTransform().p;

			// Offset rotation


			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ position.x ,position.y}, GetRotation());
			m_Dirty = false;
		}
		else
		{
			auto pos = m_pRigidbodyComponent->GetBody()->GetPosition();;
			m_WorldTransform.position = glm::vec3{ pos.x, pos.y , 0.f};
		}
	}
}

void dae::TransformComponent::Render() const
{
}

void dae::TransformComponent::RenderGUI()
{
	if (ImGui::InputValue("Position", m_WorldTransform.position))
	{
		if (m_pRigidbodyComponent != nullptr)
			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ m_WorldTransform.position.x , m_WorldTransform.position.y }, 0.0f);
	}

	ImGui::InputValue("Rotation", m_WorldTransform.rotation);
	ImGui::InputValue("Scale", m_WorldTransform.scale);
}

dae::TransformComponent::Transform& dae::TransformComponent::GetTransform()
{
	m_Dirty = true;
	return m_WorldTransform;
}

dae::TransformComponent::Transform& dae::TransformComponent::GetWorldTransform()
{
	m_Dirty = true;
	return m_WorldTransform;
}

dae::TransformComponent::Transform& dae::TransformComponent::GetLocalTransform()
{
	m_Dirty = true;
	return m_LocalTransform;
}

const dae::TransformComponent::Transform& dae::TransformComponent::GetTransformConst() const
{
	return m_WorldTransform;
}

const dae::TransformComponent::Transform& dae::TransformComponent::GetWorldTransformConst() const
{
	return m_WorldTransform;
}

const dae::TransformComponent::Transform& dae::TransformComponent::GetLocalTransformConst() const
{
	return m_LocalTransform;
}
