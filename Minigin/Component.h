#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component(GameObject* pGameObject) : m_pGameObject{ pGameObject } {};
		virtual ~Component() = default;

		virtual void Start() {};
		virtual void Update() {};
		virtual void Render() const {};
#ifdef _DEBUG
		virtual void RenderGUI() {};
#endif // _DEBUG


	protected:
		GameObject* m_pGameObject;
	};
}

