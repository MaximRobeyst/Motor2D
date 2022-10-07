#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include <backends/imgui_impl_opengl2.h>
#include <backends/imgui_impl_sdl.h>

void dae::SceneManager::Start()
{
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}

	// Remove the scene only after the update function insuring the update is done
	for(auto i : m_ScenesToRemove)
	{
		FinalRemoveScene(i);
	}
	m_ScenesToRemove.clear();
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderDebug()
{
	for (const auto& scene : m_Scenes)
	{
		scene->RenderDebug();
	}
}

#ifdef _DEBUG
void dae::SceneManager::RenderGUI(SDL_Window* /*window*/)
{
	for (const auto& scene : m_Scenes)
	{
		scene->RenderGUI();
	}
}
#endif // _DEBUG

void dae::SceneManager::FinalRemoveScene(int index)
{
	if (m_Scenes.size() == 0) return;

	if (m_Scenes.size() == 1)
	{
		m_Scenes.clear();
		return;
	}

	if (index == static_cast<int>( m_Scenes.size()) - 1)
	{
		delete m_Scenes[index];
		m_Scenes.pop_back();
		return;
	}

	delete m_Scenes[index];
	std::swap(m_Scenes[index], m_Scenes[m_Scenes.size() - 1]);
	m_Scenes.pop_back();
}


dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = new Scene(name);
	m_Scenes.push_back(scene);

	return *scene;
}

void dae::SceneManager::AddScene(Scene* pScene)
{
	m_Scenes.push_back(pScene);
}

void dae::SceneManager::RemoveScene(int index)
{
	m_ScenesToRemove.emplace(index);
}

void dae::SceneManager::RemoveScene(const std::string& sceneName, bool deleteInstant)
{
	auto iter = std::find_if(m_Scenes.begin(), m_Scenes.end(), [sceneName](Scene* pScene) {
		return pScene->GetName() == sceneName;
		});

	if (deleteInstant)
	{
		FinalRemoveScene(static_cast<int>(std::distance(m_Scenes.begin(), iter)));
		return;
	}

	m_ScenesToRemove.emplace(static_cast<int>(std::distance(m_Scenes.begin(), iter)));
}

void dae::SceneManager::Cleanup()
{
	for (const auto& scene : m_Scenes)
	{
		delete scene;
	}
}

dae::Scene* dae::SceneManager::GetScene(int index)
{
	if (std::find(m_ScenesToRemove.begin(), m_ScenesToRemove.end(), index) != m_ScenesToRemove.end())
		return GetScene(index + 1);

	return m_Scenes[index];
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name)
{
	auto iter = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](Scene* pScene) {
		return pScene->GetName() == name;
		});

	assert(iter != m_Scenes.end());

	return *iter;
}
