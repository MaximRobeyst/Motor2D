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

		virtual void RenderGUI() {};

		virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& /*writer*/) {};
		virtual void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) {};

		void SetGameObject(GameObject* pGameObject);
		GameObject* GetGameObject() const;

	protected:
		GameObject* m_pGameObject;
	};
}

