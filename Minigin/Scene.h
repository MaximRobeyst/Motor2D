#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include <box2d.h>

class CollisionHandler;
namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(dae::GameObject* object);

		std::shared_ptr<b2World> GetPhysicsWorld() const;

		void Start();
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
		explicit Scene(const std::string& name, const b2Vec2& gravity = b2Vec2{ 0.f, 0.f });

		std::string m_Name;
		std::vector < dae::GameObject*> m_pObjects{};
		std::shared_ptr<b2World> m_PhysicsWorld;

		CollisionHandler* m_pCollisionHandler{ nullptr };

		static unsigned int m_IdCounter; 
	};

}
