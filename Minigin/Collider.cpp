#include "MiniginPCH.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "SpriteRendererComponent.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RigidbodyComponent.h"

dae::Creator<dae::Component, dae::ColliderComponent> s_TranformComponentCreate{};

dae::ColliderComponent::ColliderComponent()

{
}

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject)
	: Component(pGameObject)
	, m_DynamicBox{new b2PolygonShape()}
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();
	m_pRigidbody = pGameObject->GetComponent<RigidbodyComponent>();

	auto pTextureComponent = pGameObject->GetComponent<SpriteRendererComponent>();
	if (pTextureComponent != nullptr)
	{
		m_Width = pTextureComponent->GetTexture()->GetSize().x * m_pTransform->GetScale().x;
		m_Height = pTextureComponent->GetTexture()->GetSize().y * m_pTransform->GetScale().y;
	}

	m_Center.x = (m_Width / 2);
	m_Center.y = (m_Height / 2);
	m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2, b2Vec2{ m_Center.x, m_Center.y }, 0.0f);
}

dae::ColliderComponent::ColliderComponent(dae::GameObject* pGameObject, float width, float height, glm::vec2 center)
	: Component(pGameObject)
	, m_Center{center}
	, m_DynamicBox{ new b2PolygonShape() }
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();

	m_Width = (width * m_pTransform->GetScale().x);
	m_Height = (height * m_pTransform->GetScale().y);

	if (m_Center == glm::vec2{})
	{
		m_Center.x = (m_Width / 2);
		m_Center.y = (m_Height / 2);
	}
	else
	{
		m_Center.x *= m_pTransform->GetScale().x;
		m_Center.y *= m_pTransform->GetScale().y;
	}

	m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2 , b2Vec2{m_Center.x, m_Center.y}, 0.0f);
}

dae::ColliderComponent::~ColliderComponent()
{
	delete m_DynamicBox;
}

void dae::ColliderComponent::Start()
{
	if (m_pTransform == nullptr)
		m_pTransform = m_pGameObject->GetComponent<dae::TransformComponent>();

	m_pRigidbody = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
}

void dae::ColliderComponent::Update()
{
}

void dae::ColliderComponent::Render() const
{
}

void dae::ColliderComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("Box");
	writer.StartObject();
	writer.Key("x");
	writer.Double(static_cast<double>(m_Center.x));
	writer.Key("y");
	writer.Double(static_cast<double>(m_Center.y));
	writer.Key("w");
	writer.Double(static_cast<double>(m_Width));
	writer.Key("h");
	writer.Double(static_cast<double>(m_Height));
	writer.EndObject();
	writer.EndObject();
}

void dae::ColliderComponent::Deserialize(GameObject* pGameOject, rapidjson::Value& value)
{
	m_pGameObject = pGameOject;

	m_Center = glm::vec2{ static_cast<float>(value["Box"]["x"].GetDouble()), static_cast<float>(value["Box"]["y"].GetDouble()) };
	m_Width = static_cast<float>(value["Box"]["w"].GetDouble());
	m_Height = static_cast<float>(value["Box"]["h"].GetDouble());

	m_DynamicBox = new b2PolygonShape();
	m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2, b2Vec2{ m_Center.x, m_Center.y }, 0.0f);
}

glm::vec2 dae::ColliderComponent::GetSize() const
{
	return glm::vec2{m_Width, m_Height};
}

#ifdef _DEBUG
void dae::ColliderComponent::RenderGUI()
{
	bool changed{ false };

	ImGui::Text("Collider");
	if (ImGui::InputFloat("Width", &m_Width))
		changed = true;
	if (ImGui::InputFloat("Height", &m_Height))
		changed = true;
	float center[2]{ m_Center.x, m_Center.y };
	if (ImGui::InputFloat2("Center", center))
		changed = true;

	if (changed)
	{
		m_Center.x = center[0];
		m_Center.y = center[1];

		m_DynamicBox->SetAsBox(m_Width / 2, m_Height / 2, b2Vec2{ m_Center.x, m_Center.y }, 0.0f);
		m_pRigidbody->ChangeShape(m_DynamicBox);
	}
}
#endif // _DEBUG

b2PolygonShape* dae::ColliderComponent::GetDynamicBox() const
{
	return m_DynamicBox;
}