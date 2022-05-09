#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"

#include <memory>
#include <vector>
#include <string>

namespace dae
{
	class GameObject final : public SceneObject
	{
	public:
		GameObject(const std::string& name = "Gameobject");
		GameObject(GameObject* pParent, std::vector<GameObject*> pChildren = std::vector<GameObject*>{});
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start();
		void Update() override;
		void Render() const override;

#ifdef _DEBUG
		void RenderGUI();
#endif // _DEBUG


		void AddComponent(Component* component);
		template<typename T>
		T* GetComponent() const
		{
			for (int i = 0; i < m_pComponents.size(); ++i)
			{
				T* comp{ dynamic_cast<T*>(m_pComponents[i])};
				if (comp != nullptr)
					return comp;
			}
			return nullptr;
		}

		// SceneGraph
		void SetParent(GameObject* pParent, bool worldPositionStays = true);
		GameObject* GetParent() const;

		GameObject* GetChildFromIndex(int i) const;
		size_t GetAmountOfChildren() const;

		void SetTag(std::string tag);
		std::string GetTag() const;

		std::string GetName() const;


	private:
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		// Extra data
		std::string m_Name{};
		std::string m_Tag{};

		// Components
		std::vector<Component*> m_pComponents{};

		// Scene Graph
		GameObject* m_pParent{};
		std::vector<GameObject*> m_pChildren{};

	};
}
