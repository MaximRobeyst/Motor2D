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
	for (int i = 0; i < m_ScenesToRemove.size(); ++i)
	{
		FinalRemoveScene(m_ScenesToRemove[i]);
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

#ifdef _DEBUG
void dae::SceneManager::RenderGUI(SDL_Window* window)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	for (const auto& scene : m_Scenes)
	{
		scene->RenderGUI();
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
#endif // _DEBUG

void dae::SceneManager::FinalRemoveScene(int index)
{
	if (index == m_Scenes.size() - 1)
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
	m_ScenesToRemove.push_back(index);
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
	return m_Scenes[index];
}
