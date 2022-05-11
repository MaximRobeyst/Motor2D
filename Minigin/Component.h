#pragma once

#include <rapidjson.h>
#include <prettywriter.h>

#include <rapidjson.h>
#include <document.h>

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component() = default;
		Component(GameObject* pGameObject) : m_pGameObject{ pGameObject } {};
		virtual ~Component() = default;

		virtual void Start() {};
		virtual void Update() {};
		virtual void Render() const {};
#ifdef _DEBUG
		virtual void RenderGUI() {};
#endif // _DEBUG

		virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) = 0 ;
		virtual void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) {};

		void SetGameObject(GameObject* pGameObject);

	protected:
		GameObject* m_pGameObject;
	};
}

