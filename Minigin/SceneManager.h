#pragma once
#include "Singleton.h"

#include <string>
#include <memory>
#include<SDL.h>
#include <vector>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
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
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<Scene*> m_Scenes;
	};
}
