#pragma once
#include "Singleton.h"

#include <string>
#include <memory>
#include<SDL.h>
#include <vector>
#include <set>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void AddScene(Scene* pScene);
		Scene* GetScene(int index);

		void RemoveScene(int index);
		void Cleanup();

		void Start();
		void Update();
		void Render();
#ifdef _DEBUG
		void RenderGUI(SDL_Window* window);
#endif // _DEBUG

	private:
		void FinalRemoveScene(int index);


		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<Scene*> m_Scenes{};
		std::set<int> m_ScenesToRemove{};
		char m_TargetSceneName[128]{};
	};
}
