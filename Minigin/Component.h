#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component(GameObject* pGameObject) : m_pGameObject{ pGameObject } {};
		virtual ~Component() = default;

		virtual void Update() {};
		virtual void Render() const {};

	protected:
		GameObject* m_pGameObject;
	};
}

