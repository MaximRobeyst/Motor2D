#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include <imgui.h>

dae::Creator<dae::Component, dae::TransformComponent> s_TranformComponentCreate{};

dae::TransformComponent::TransformComponent(GameObject* pGameobject, glm::vec3 position, glm::vec2 scale)
	: Component{pGameobject}
	, m_Transform{ position, position, 0.0f, scale }
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
	writer.Double(static_cast<double>(m_Transform.worldPosition.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_Transform.worldPosition.y));
	writer.Key("z");
	writer.Double(static_cast<double>(m_Transform.worldPosition.z));
	writer.EndObject();
	writer.Key("Rotation");
	writer.Double(static_cast<double>(m_Transform.rotation));
	writer.Key("Scale");
	writer.StartObject();
	writer.Key("x");
	writer.Double(static_cast<double>(m_Transform.scale.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_Transform.scale.y));
	writer.EndObject();
	writer.EndObject();
}

void dae::TransformComponent::Deserialize(GameObject* pGameObject, rapidjson::Value& value)
{
	m_pGameObject = pGameObject;
	m_Transform.worldPosition =
		glm::vec3{ 
		static_cast<float>(value["Position"]["x"].GetDouble()),
		static_cast<float>(value["Position"]["y"].GetDouble()),
		static_cast<float>(value["Position"]["z"].GetDouble())
	};
	m_Transform.rotation = static_cast<float>(value["Rotation"].GetDouble());

	m_Transform.scale =
		glm::vec2{
		static_cast<float>(value["Scale"]["x"].GetDouble()),
		static_cast<float>(value["Scale"]["y"].GetDouble())
	};
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

void dae::TransformComponent::SetPosition(const b2Vec2& position)
{
	m_Dirty = true;
	m_Transform.worldPosition.x = position.x;
	m_Transform.worldPosition.y = position.y;
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

void dae::TransformComponent::SetRotation(float rot)
{
	m_Dirty = true;
	m_Transform.rotation = rot;
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
		if (m_Dirty && m_Transform.rotation != 0)
		{
			auto body = m_pRigidbodyComponent->GetBody()->GetTransform();


			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ body.p.x ,body.p.y}, m_Transform.rotation);
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

void dae::TransformComponent::RenderGUI()
{
	float position[3] = { m_Transform.worldPosition.x, m_Transform.worldPosition.y, m_Transform.worldPosition.z };
	if (ImGui::InputFloat3("Position", position))
	{
		m_Transform.worldPosition = glm::vec3{ position[0], position[1], position[2] };
		if (m_pRigidbodyComponent != nullptr)
			m_pRigidbodyComponent->GetBody()->SetTransform(b2Vec2{ position[0], position[1] }, 0.0f);
	}

	float rotation = { m_Transform.rotation };
	if (ImGui::InputFloat("Rotation", &rotation))
		m_Transform.rotation = rotation;

	float scale[2] = { m_Transform.scale.x, m_Transform.scale.y};
	if (ImGui::InputFloat2("Scale", scale))
		m_Transform.scale = glm::vec2{ scale[0], scale[1]};
	//ImGui::InputFloat("Rotation", &m_Transform.rotation);
	//ImGui::InputFloat2("Scale", glm::value_ptr(m_Transform.scale));
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
