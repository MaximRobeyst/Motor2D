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
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update() override;
		void Render() const override;

		void AddComponent(Component* component);
		template<typename T>
		T* GetComponent() const
		{
			for (auto component : m_pComponents)
			{
				if (typeid(component) == typeid(T))
					static_cast<T*>(component);
			}
			return nullptr;
		}


	private:
		std::vector<Component*> m_pComponents;
	};
}
