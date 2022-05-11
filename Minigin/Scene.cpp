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

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, const b2Vec2& gravity)
	: m_Name(name), 
	m_PhysicsWorld{ std::make_shared<b2World>(gravity) }
{
	m_pCollisionHandler = new CollisionHandler();
	m_PhysicsWorld->SetContactListener(m_pCollisionHandler);
}

Scene::~Scene()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); ++iter)
		delete* iter;

	delete m_pCollisionHandler;
	m_pObjects.clear();
}

void Scene::AddGameObject(dae::GameObject* object)
{
	m_pObjects.push_back(object);
}

void dae::Scene::RemoveGameObject(dae::GameObject* object)
{
	if (object->GetParent() != nullptr)
		object->SetParent(nullptr);

	auto objectToRemove = std::find(m_pObjects.begin(), m_pObjects.end(), object);
	m_pObjects.erase(objectToRemove);

	m_pObjectsToDelete.push_back(object);
}

std::shared_ptr<b2World> dae::Scene::GetPhysicsWorld() const
{
	return m_PhysicsWorld;
}

void dae::Scene::Start()
{
	for (auto& object : m_pObjects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	// Update physics world
	m_PhysicsWorld->Step(GameTime::GetInstance()->GetElapsed(), 6, 2);

	// Update all objects
	for(int i = 0; i < m_pObjects.size(); ++i)
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
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
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
				Deserialize();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
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
	}

	// Scene graph
	if (ImGui::CollapsingHeader((m_Name + " Gameobjects").c_str()))
	{
		for (auto& gameobject : m_pObjects)
		{
			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			if(gameobject->GetAmountOfChildren() == 0) base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet;

			if (gameobject->GetParent() == nullptr && ImGui::TreeNode(gameobject->GetName().c_str()))
			{
				for (int i = 0; i < gameobject->GetAmountOfChildren(); ++i)
				{
					if (ImGui::TreeNode(gameobject->GetChildFromIndex(i)->GetName().c_str()))
					{
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();

}
void dae::Scene::Serialize()
{
	std::ofstream levelFile{ "../Data/Scenes/Scene.json" };
	if (!levelFile.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		return;
	}
	rapidjson::StringBuffer outputFile{};
	rapidjson::PrettyWriter< rapidjson::StringBuffer> writer(outputFile);

	writer.StartObject();
	writer.Key("sceneName");
	writer.String(m_Name.c_str());
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
	writer.EndObject();


	levelFile << outputFile.GetString();
	levelFile.close();
}
dae::Scene* dae::Scene::Deserialize()
{
	std::ifstream levelFile{ "../Data/Scenes/Scene.json" };
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

	SceneManager::GetInstance().RemoveScene(0);
	SceneManager::GetInstance().AddScene(pScene);

	return pScene;
}
#endif
