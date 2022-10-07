#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Scene.h"
#include "RigidbodyComponent.h"

#include <misc/cpp/imgui_stdlib.h>

#include <fstream>
#include <istreamwrapper.h>

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
	//if (m_pParent != nullptr)
	//	SetParent(nullptr);

	for (auto child : m_pChildren)
		delete child;
	m_pChildren.clear();
	
	for (int i = 0; i < static_cast<int>(m_pComponents.size()); ++i)
		delete m_pComponents[i];

	m_pComponents.clear();

}

void dae::GameObject::Start()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Start();
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		(*iter)->Start();
	}

	m_pTransformComponent = GetComponent<TransformComponent>();
}

void dae::GameObject::Update()
{
	if (!m_Enabled)
		return;

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Update();
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		(*iter)->Update();
	}

	if (m_MarkedDelete) 
		ImmidiateDestroy(this);
}

void dae::GameObject::Render() const
{
	if (!m_Enabled)
		return;

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Render();
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
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

	if (ImGui::Button("Save as prefab..."))
	{
		SaveAsPrefab();
	}
	if (ImGui::Button("Delete"))
	{
		m_pCurrentScene->RemoveGameObject(this);
	}

	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		if (ImGui::CollapsingHeader(typeid(**iter).name()))
			(*iter)->RenderGUI();
	}

	ImGui::EndChild();
}
#endif // _DEBUG

void dae::GameObject::Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer)
{
	if (!m_Serialize) return;

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
			gameobject->Serialize(writer);
		}
		writer.EndArray();
	//}

	writer.Key("Components");
	writer.StartArray();
	for (auto& component : m_pComponents)
	{
		component->Serialize(writer);
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		(*iter)->Serialize(writer);
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

dae::GameObject* dae::GameObject::Deserialize(Scene* pScene, const std::string& filename)
{
	std::ifstream prefabFile{ {"../Data/Prefabs/" + filename + ".json"} };
	if (!prefabFile.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		return nullptr;
	}
	rapidjson::IStreamWrapper isw{ prefabFile };
	rapidjson::Document prefabDocument{};
	prefabDocument.ParseStream(isw);

	return Deserialize(pScene, prefabDocument);
}



void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
	component->SetGameObject(this);
}



void dae::GameObject::Destroy(GameObject* pGameobject)
{
	if (pGameobject->m_pParent != nullptr)
		pGameobject->SetParent(nullptr);

	pGameobject->GetScene()->RemoveGameObject(pGameobject);

	pGameobject->m_MarkedDelete = true;
}

void dae::GameObject::DestroyInstant(GameObject* pGameobject)
{
	if (pGameobject->m_pParent != nullptr)
		pGameobject->SetParent(nullptr);

	pGameobject->GetScene()->RemoveGameObject(pGameobject);

	dae::GameObject::ImmidiateDestroy(pGameobject);
}

void dae::GameObject::SetParent(GameObject* pParent, bool worldPositionStays)
{
	if (pParent == this) return;
	if (m_pCurrentScene == nullptr) m_pCurrentScene = dae::SceneManager::GetInstance().GetScene(0);

	// Remove itself as a child from the previous parent
	if (m_pParent != nullptr)
		m_pParent->RemoveChild(this);
	else
		m_pCurrentScene->RemoveGameObject(this);

	// Set The given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if (pParent != nullptr)
		pParent->AddChild(this);
	else
		m_pCurrentScene->AddGameObject(this);

	GetTransform()->SetParent(pParent, worldPositionStays);
	
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::SaveAsPrefab()
{
	std::ofstream prefabFile{ "../Data/Prefabs/" + m_Name + ".json" };
	if (!prefabFile.is_open())
	{
		std::cout << "Could not open file" << std::endl;
	}

	rapidjson::StringBuffer outputFile{};
	rapidjson::PrettyWriter <rapidjson::StringBuffer> writer(outputFile);

	Serialize(writer);

	prefabFile << outputFile.GetString();
	prefabFile.close();
}

void dae::GameObject::ImmidiateDestroy(GameObject* gameobject)
{
	delete gameobject;
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

	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject), m_pChildren.end());
	//m_pChildren.erase()

	//pGameObject->SetParent(nullptr);

	// Update Position
}

dae::GameObject* dae::GameObject::GetChildFromIndex(int i) const
{
	assert(i < static_cast<int>(m_pChildren.size()));

	return m_pChildren[i];
}

int dae::GameObject::GetAmountOfChildren() const
{
	return static_cast<int>(m_pChildren.size());
}

void dae::GameObject::SetScene(Scene* pScene)
{
	m_pCurrentScene = pScene;
}

dae::Scene* dae::GameObject::GetScene() const
{
	if (m_pCurrentScene == nullptr)
		return m_pParent->GetScene();

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

dae::TransformComponent* dae::GameObject::GetTransform()
{
	if (m_pTransformComponent == nullptr)
		m_pTransformComponent = GetComponent<TransformComponent>();

	return m_pTransformComponent;
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

	if (dae::RigidbodyComponent* pRigidbody = GetComponent<dae::RigidbodyComponent>(); pRigidbody != nullptr)
	{
		pRigidbody->SetSensor(!newValue);
	}
}

bool dae::GameObject::IsSerializable() const
{
	return m_Serialize;
}

void dae::GameObject::SetSerializable(bool b)
{
	m_Serialize = b;
}

void dae::GameObject::SetId(int id)
{
	m_InstanceId = id;
}

int dae::GameObject::GetId() const
{
	return m_InstanceId;
}
