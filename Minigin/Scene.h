#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(dae::GameObject* object);

		void Update();
		void Render() const;
#ifdef _DEBUG
		void RenderGUI();
#endif // _DEBUG


		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < dae::GameObject*> m_pObjects{};

		static unsigned int m_IdCounter; 
	};

}
