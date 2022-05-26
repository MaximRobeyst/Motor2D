#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <imgui.h>
#include "GameTime.h"
#include "CollisionHandler.h"

#include <rapidjson.h>
#include <document.h>
#include <ostreamwrapper.h>
#include <fstream>
#include <writer.h>
#include <prettywriter.h>
#include <istreamwrapper.h>
#include "InputManager.h"

#include "PhysicsDebugDraw.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, const b2Vec2& gravity)
	: m_Name(name), 
	m_PhysicsWorld{ std::make_shared<b2World>(gravity) }
{
	m_pCollisionHandler = new CollisionHandler();

	m_PhysicsWorld->SetContactListener(m_pCollisionHandler);


	//m_pPhysicsDebugDraw = new PhysicsDebugDraw();
	//
	//uint32 flags = 0;
	//flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_centerOfMassBit;
	//m_pPhysicsDebugDraw->SetFlags(flags);
	//
	//m_PhysicsWorld->SetDebugDraw(m_pPhysicsDebugDraw);
}

#ifdef _DEBUG
void dae::Scene::RenderGameObjectGUI(GameObject* pGameobject)
{
	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	bool nodeOpen = ImGui::TreeNodeEx(pGameobject->GetName().c_str(), baseFlags);
	if (ImGui::IsItemClicked() && ImGui::IsItemToggledOpen())
		m_pSelectedObject = pGameobject;

	if(nodeOpen)
	{
		baseFlags |= ImGuiTreeNodeFlags_Selected;
		for (int i = 0; i <  static_cast<int>(pGameobject->GetAmountOfChildren()); ++i)
		{
			RenderGameObjectGUI(pGameobject->GetChildFromIndex(i));
		}
		ImGui::TreePop();
	}

}
#endif // _DEBUG

Scene::~Scene()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); ++iter)
		delete* iter;

	delete m_pCollisionHandler;
	if(m_pPhysicsDebugDraw != nullptr)
		delete m_pPhysicsDebugDraw;
	m_pObjects.clear();
}

void Scene::AddGameObject(dae::GameObject* object)
{
	m_pObjects.emplace_back(object);
	object->SetScene(this);
	if(object->GetId() == 0)
		object->SetId( static_cast<int>( m_pObjects.size()));

	if(m_Started)
		object->Start();
}

void dae::Scene::RemoveGameObject(dae::GameObject* object)
{
	if (object->GetParent() != nullptr)
		object->SetParent(nullptr);

	auto objectToRemove = std::find(m_pObjects.begin(), m_pObjects.end(), object);
	m_pObjects.erase(objectToRemove);

	m_pObjectsToDelete.push_back(object);
}

GameObject* dae::Scene::FindGameobjectWithTag(const std::string& tag)
{
	auto objectWithTag = std::find_if(m_pObjects.begin(), m_pObjects.end(), [&](GameObject* pGameobject) {
		return pGameobject->GetTag() == tag;
		});
	if (objectWithTag != m_pObjects.end())
		return *objectWithTag;
	
	return nullptr;
}

GameObject* dae::Scene::GetGameobjectFromId(int id)
{
	return *std::find_if(m_pObjects.begin(), m_pObjects.end(), [&](GameObject* pGameobject)
		{
			return pGameobject->GetId() == id;
		});
}

std::shared_ptr<b2World> dae::Scene::GetPhysicsWorld() const
{
	return m_PhysicsWorld;
}

bool dae::Scene::GetDebugPhysics() const
{
	return m_DebugPhysics;
}

void dae::Scene::Start()
{
	m_Started = true;
	for(size_t i =0 ; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Start();
	}
}

void Scene::Update()
{
	assert(m_Started);

	// Update physics world
	m_PhysicsWorld->Step(GameTime::GetInstance()->GetElapsed(), 6, 2);

	// Update all objects
	for(size_t i = 0; i < m_pObjects.size(); ++i)
	{
		 m_pObjects[i]->Update();
	}

	// Delete marked for delete objects
	for (size_t i = 0; i < m_pObjectsToDelete.size(); ++i)
		delete m_pObjectsToDelete[i];
	m_pObjectsToDelete.clear();
}

void Scene::Render() const
{
	assert(m_Started);

	for (const auto& object : m_pObjects)
	{
		object->Render();
	}

	//m_PhysicsWorld->DebugDraw();
}

#ifdef _DEBUG
void dae::Scene::RenderGUI()
{
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_MenuBar;

	ImGui::Begin(m_Name.c_str(), (bool*)0, windowFlags);
	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
				Serialize();
			if (ImGui::MenuItem("Load"))
			{
				Deserialize(m_Name);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	static char filename[128] = "scene.json";
	ImGui::InputText("Scene filename: ", filename, 128);
	if (ImGui::Button("Save"))
	{
		Serialize(filename);
	}
	if (ImGui::Button("Load"))
	{
		Deserialize(filename);
	}

	// Scene Settings
	if (ImGui::CollapsingHeader((m_Name + " Scene settings").c_str()))
	{
		auto gravity = m_PhysicsWorld->GetGravity();
		float vel[2] = { gravity.x, gravity.y };
		if (ImGui::DragFloat2("Gravity: ", vel))
		{
			m_PhysicsWorld->SetGravity(b2Vec2{ vel[0], vel[1] });
		}

		ImGui::Checkbox("Draw debug phyiscs", &m_DebugPhysics);
	}

	// Scene graph
	if (ImGui::CollapsingHeader((m_Name + " Gameobjects").c_str()))
	{
		for (auto& gameobject : m_pObjects)
		{
			if(gameobject->GetParent() == nullptr)
				RenderGameObjectGUI(gameobject);
		}
	}

	if (m_pSelectedObject != nullptr)
		m_pSelectedObject->RenderGUI();

	ImGui::End();

}
#endif

void dae::Scene::Serialize(const std::string& name)
{
	std::ofstream levelFile{ "../Data/Scenes/" + (name == " " ? m_Name : name) + ".json"};
	if (!levelFile.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		return;
	}
	rapidjson::StringBuffer outputFile{};
	rapidjson::PrettyWriter< rapidjson::StringBuffer> writer(outputFile);

	writer.StartObject();
	writer.Key("sceneName");
	writer.String((name == " " ? m_Name : name).c_str());

	writer.Key("gameobjectCount");
	writer.Int((int) m_pObjects.size());
	writer.Key("Gravity");
	writer.StartObject();
	writer.Key("X");
	writer.Double((double)m_PhysicsWorld->GetGravity().x);
	writer.Key("Y");
	writer.Double((double)m_PhysicsWorld->GetGravity().y);
	writer.EndObject();

	writer.Key("gameobjects");
	writer.StartArray();
	for (auto& gameobject : m_pObjects)
	{
		if (gameobject->GetParent() != nullptr)
			continue;
		gameobject->Sertialize(writer);
	}
	writer.EndArray();
	writer.Key("InputManager");
	dae::InputManager::GetInstance().Serialize(writer);
	writer.EndObject();


	levelFile << outputFile.GetString();
	levelFile.close();
}
dae::Scene* dae::Scene::Deserialize(const std::string& sceneFile)
{
	std::ifstream levelFile{ ("../Data/Scenes/" + sceneFile + ".json")};
	if (!levelFile.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		return nullptr;
	}
	rapidjson::IStreamWrapper isw{ levelFile };
	rapidjson::Document levelDocument{};
	levelDocument.ParseStream(isw);


	std::string sceneName = levelDocument["sceneName"].GetString();
	b2Vec2 gravity{};
	gravity.x = static_cast<float>(levelDocument["Gravity"]["X"].GetDouble());
	gravity.y = static_cast<float>( levelDocument["Gravity"]["Y"].GetDouble());

	dae::Scene* pScene = new dae::Scene(sceneName, gravity);
	for (auto& gamobject : levelDocument["gameobjects"].GetArray())
	{
		pScene->AddGameObject(GameObject::Deserialize(pScene, gamobject));
	}
	if (levelDocument.HasMember("InputManager"))
		InputManager::GetInstance().Deserialize(levelDocument["InputManager"], pScene);

	SceneManager::GetInstance().RemoveScene(0);
	SceneManager::GetInstance().AddScene(pScene);
	pScene->Start();

	return pScene;
}
