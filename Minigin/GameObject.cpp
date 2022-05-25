#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Scene.h"

#include <misc/cpp/imgui_stdlib.h>

dae::GameObject::GameObject(const std::string& name)
	: m_Name{name}
{
}

dae::GameObject::GameObject(GameObject* pParent, std::vector<GameObject*> pChildren)
	: m_pParent{pParent}
	, m_pChildren{pChildren}
{
	pParent->AddChild(this);
}

dae::GameObject::~GameObject()
{
	if (m_pParent != nullptr)
		SetParent(nullptr);

	m_pChildren.clear();
	

	//for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	//	delete *iter;
	for (int i = 0; i < m_pComponents.size(); ++i)
		delete m_pComponents[i];

	m_pComponents.clear();

	m_pChildren.clear();
}

void dae::GameObject::Start()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Start();
	}
}

void dae::GameObject::Update()
{
	if (!m_Enabled)
		return;

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Update();
	}
}

void dae::GameObject::Render() const
{
	if (!m_Enabled)
		return;

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Render();
	}

}

#ifdef _DEBUG
void dae::GameObject::RenderGUI()
{
	ImGui::BeginChild(m_Name.c_str(), ImVec2{ ImGui::GetContentRegionAvail().x, 250.f }, true);

	ImGui::Text(m_Name.c_str());
	if (ImGui::Checkbox("Enabled", &m_Enabled))
		SetEnabled(m_Enabled);

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		if (ImGui::CollapsingHeader(typeid(**iter).name()))
			(*iter)->RenderGUI();
	}
	ImGui::EndChild();
}
#endif // _DEBUG

void dae::GameObject::Sertialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Id");
	writer.Int(m_InstanceId);
	writer.Key("Name");
	writer.String(m_Name.c_str());
	writer.Key("Tag");
	writer.String(m_Tag.c_str());

	//if (m_pChildren.size() > 0)
	//{
		writer.Key("Children");
		writer.StartArray();
		for (auto& gameobject : m_pChildren)
		{
			gameobject->Sertialize(writer);
		}
		writer.EndArray();
	//}

	writer.Key("Components");
	writer.StartArray();
	for (auto& component : m_pComponents)
	{
		component->Serialize(writer);
	}
	writer.EndArray();

	writer.EndObject();
}

dae::GameObject* dae::GameObject::Deserialize(Scene* pScene, rapidjson::Value& value)
{
	GameObject* pGameobject = new GameObject(value["Name"].GetString());
	pGameobject->SetTag(value["Tag"].GetString());
	pGameobject->SetId(value["Id"].GetInt());
	pGameobject->SetScene(pScene);

	for (auto& gameobject : value["Children"].GetArray())
	{
		//std::cout << gameobject["Name"].GetString() << std::endl;
		auto pChild = GameObject::Deserialize(pScene, gameobject);
		pScene->AddGameObject(pChild);
		pChild->SetParent(pGameobject);
	}

	for (auto& component : value["Components"].GetArray())
	{
		std::cout << component["name"].GetString() << std::endl;
		auto comp = Factory<Component>::GetInstance().Create(component["name"].GetString());
		if (comp == nullptr)
			continue;

		pGameobject->AddComponent(comp);
		comp->Deserialize(pGameobject, component);
		//comp->Start();

	}

	return pGameobject;
}



void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}

void dae::GameObject::SetParent(GameObject* pParent, bool /*worldPositionStays*/)
{
	if (pParent == this) return;

	// Remove itself as a child from the previous parent
	if (m_pParent != nullptr)
		m_pParent->RemoveChild(this);

	// Set The given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if(pParent != nullptr)
		pParent->AddChild(this);

	// TODO: Update position transform
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);

	// if already has a parent remove if from the other parent
	// Set transform relative (Check integrity of scenegraph)

}

void dae::GameObject::RemoveChild(GameObject* pGameObject)
{
	if (m_pChildren.size() <= 0) return;

	auto it = std::remove_if(m_pChildren.begin(), m_pChildren.end(), [&](GameObject* pChild) {
		return pChild == pGameObject;
		});
	//m_pChildren.erase()

	//pGameObject->SetParent(nullptr);

	// Update Position
}

dae::GameObject* dae::GameObject::GetChildFromIndex(int i) const
{
	assert(i < static_cast<int>(m_pChildren.size()));

	return m_pChildren[i];
}

size_t dae::GameObject::GetAmountOfChildren() const
{
	return m_pChildren.size();
}

void dae::GameObject::SetScene(Scene* pScene)
{
	m_pCurrentScene = pScene;
}

dae::Scene* dae::GameObject::GetScene() const
{
	return m_pCurrentScene;
}

void dae::GameObject::SetTag(std::string tag)
{
	m_Tag = tag;
}

std::string dae::GameObject::GetTag() const
{
	return m_Tag;
}

std::string dae::GameObject::GetName() const
{
	return m_Name;
}

void dae::GameObject::SetName(const std::string& newName)
{
	m_Name = newName;
}

bool dae::GameObject::IsEnabled() const
{
	return m_Enabled;
}

void dae::GameObject::SetEnabled(bool newValue, bool AffectChildren)
{
	m_Enabled = newValue;
	if (!AffectChildren) return;

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		(*iter)->SetEnabled(newValue, AffectChildren);
	}
}

void dae::GameObject::SetId(int id)
{
	m_InstanceId = id;
}

int dae::GameObject::GetId() const
{
	return m_InstanceId;
}
