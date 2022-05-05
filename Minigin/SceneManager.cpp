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



dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = new Scene(name);
	m_Scenes.push_back(scene);

	return *scene;
}

void dae::SceneManager::RemoveScene(int index)
{
	delete m_Scenes[index];
	m_Scenes.erase(m_Scenes.begin() + index);
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
