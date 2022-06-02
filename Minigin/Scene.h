#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include <box2d.h>

class CollisionHandler;
class PhysicsDebugDraw;
namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void AddGameObject(dae::GameObject* object);
		void RemoveGameObject(dae::GameObject* object);

		GameObject* FindGameobjectWithTag(const std::string& tag);
		GameObject* GetGameobjectFromId(int id);

		std::shared_ptr<b2World> GetPhysicsWorld() const;
		bool GetDebugPhysics() const;

		std::string GetName() const;

		void Start();
		void Update();
		void Render() const;
		void RenderDebug() const;
#ifdef _DEBUG
		void RenderGUI();
#endif // _DEBUG

		void Serialize(const std::string& name = " ");
		static Scene* Deserialize(const std::string& sceneFile);


		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name, const b2Vec2& gravity = b2Vec2{ 0.f, 0.f });

#ifdef _DEBUG
		void RenderGameObjectGUI(GameObject* pGameobject);
		
		GameObject* m_pSelectedObject;

#endif // _DEBUG

		std::string m_Name;
		std::vector < dae::GameObject*> m_pObjects{};
		std::vector < dae::GameObject*> m_pObjectsToDelete{};
		std::shared_ptr<b2World> m_PhysicsWorld;

		CollisionHandler* m_pCollisionHandler{ nullptr };
		PhysicsDebugDraw* m_pPhysicsDebugDraw{ nullptr };

		static unsigned int m_IdCounter; 
		bool m_Started{ false };
		bool m_DebugPhysics{ true };
	};

}
