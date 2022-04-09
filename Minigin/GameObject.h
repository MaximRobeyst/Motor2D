#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"

#include <memory>
#include <vector>

namespace dae
{

	// todo: this should become final.
	class GameObject final : public SceneObject
	{
	public:
		GameObject() = default;
		GameObject(GameObject* pParent, std::vector<GameObject*> pChildren = std::vector<GameObject*>{});
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update() override;
		void Render() const override;

#ifdef _DEBUG
		void RenderGUI();
#endif // _DEBUG


		void AddComponent(Component* component);
		template<typename T>
		T* GetComponent() const
		{
			for (auto& component : m_pComponents)
			{
				T* comp{ dynamic_cast<T*>(component) };
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

	private:
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		std::string m_Tag{};

		std::vector<Component*> m_pComponents;

		GameObject* m_pParent{};
		std::vector<GameObject*> m_pChildren{};

	};
}
