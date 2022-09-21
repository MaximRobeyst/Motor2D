#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include <imgui.h>

dae::Creator<dae::Component, dae::TransformComponent> s_TranformComponentCreate{};

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
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("Position");
	writer.StartObject();
	writer.Key("x");
	writer.Double(static_cast<double>(m_WorldTransform.position.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_WorldTransform.position.y));
	writer.Key("z");
	writer.Double(static_cast<double>(m_WorldTransform.position.z));
	writer.EndObject();
	writer.Key("Rotation");
	writer.Double(static_cast<double>(m_WorldTransform.rotation));
	writer.Key("Scale");
	writer.StartObject();
	writer.Key("x");
	writer.Double(static_cast<double>(m_WorldTransform.scale.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_WorldTransform.scale.y));
	writer.EndObject();
	writer.EndObject();
}

void dae::TransformComponent::Deserialize(GameObject* pGameObject, rapidjson::Value& value)
{
	m_pGameObject = pGameObject;
	m_WorldTransform.position =
		glm::vec3{ 
		static_cast<float>(value["Position"]["x"].GetDouble()),
		static_cast<float>(value["Position"]["y"].GetDouble()),
		static_cast<float>(value["Position"]["z"].GetDouble())
	};
	m_WorldTransform.rotation = static_cast<float>(value["Rotation"].GetDouble());

	m_WorldTransform.scale =
		glm::vec2{
		static_cast<float>(value["Scale"]["x"].GetDouble()),
		static_cast<float>(value["Scale"]["y"].GetDouble())
	};
}

const glm::vec3& dae::TransformComponent::GetPosition() const 
{
	return m_WorldTransform.position; 
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
		if (m_Dirty && m_WorldTransform.rotation != 0)
		{
			auto body = m_pRigidbodyComponent->GetBody();
			auto position = body->GetTransform().p;

			// Offset rotation


			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ position.x ,position.y}, m_WorldTransform.rotation);
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
	float position[3] = { m_WorldTransform.position.x, m_WorldTransform.position.y, m_WorldTransform.position.z };
	if (ImGui::InputFloat3("Position", position))
	{
		m_WorldTransform.position = glm::vec3{ position[0], position[1], position[2] };
		if (m_pRigidbodyComponent != nullptr)
			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ position[0], position[1] }, 0.0f);
	}

	float rotation = { m_WorldTransform.rotation };
	if (ImGui::InputFloat("Rotation", &rotation))
		m_WorldTransform.rotation = rotation;

	float scale[2] = { m_WorldTransform.scale.x, m_WorldTransform.scale.y};
	if (ImGui::InputFloat2("Scale", scale))
		m_WorldTransform.scale = glm::vec2{ scale[0], scale[1]};
	//ImGui::InputFloat("Rotation", &m_Transform.rotation);
	//ImGui::InputFloat2("Scale", glm::value_ptr(m_Transform.scale));
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
