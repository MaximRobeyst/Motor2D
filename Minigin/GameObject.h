#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"

#include <memory>

namespace dae
{

	// todo: this should become final.
	class GameObject final : public SceneObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update() override;
		void Render() const override;

		template<typename T>
		T* AddComponent(T* component)
		{
			m_pComponents.push_back(component);
		}
		template<typename T>
		T* GetComponent() const
		{
			for (auto component : m_pComponents)
			{

			}

			auto component = std::find(m_pComponents.begin(), m_pComponents.end(), [](Component* component) {
				return typeid(component) == typeid(T);
				});

			if (*component != nullptr)
				return static_cast<T*>(component);

			return nullptr;
		}


	private:
		std::vector<Component*> m_pComponents;
	};
}
