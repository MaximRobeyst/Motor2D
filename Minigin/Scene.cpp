#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <imgui.h>
#include "GameTime.h"
#include "CollisionHandler.h"

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
	ImGui::Begin(m_Name.c_str());
	if (ImGui::CollapsingHeader((m_Name + " Physics settings").c_str()))
	{
		auto gravity = m_PhysicsWorld->GetGravity();
		float vel[2] = { gravity.x, gravity.y };
		if (ImGui::DragFloat2("Gravity: ", vel))
		{
			m_PhysicsWorld->SetGravity(b2Vec2{ vel[0], vel[1] });
		}
	}

	if (ImGui::CollapsingHeader((m_Name + " Gameobjects").c_str()))
	{
	}
	ImGui::End();

}
#endif
