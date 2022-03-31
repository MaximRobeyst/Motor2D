#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		std::shared_ptr<Scene> GetScene(int index);

		void Update();
		void Render();
#ifdef _DEBUG
		void RenderGUI(SDL_Window* window);
#endif // _DEBUG

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}
