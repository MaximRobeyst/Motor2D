#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); ++iter)
		delete* iter;

	m_pObjects.clear();
}

void Scene::Add(SceneObject* object)
{
	m_pObjects.push_back(object);
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

