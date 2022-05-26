#include "UIButtonComponent.h"

#include <GameObject.h>
#include <TextObject.h>
#include <InputManager.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <Renderer.h>
#include <Keyboard.h>
#include <iostream>

dae::Creator<dae::Component, UIButtonComponent> s_TranformComponentCreate{};

UIButtonComponent::UIButtonComponent(dae::GameObject* pGameobject, glm::vec4 color)
	: Component(pGameobject)
{
}

UIButtonComponent::~UIButtonComponent()
{
	delete m_pOnClickFunction;
}

void UIButtonComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("FunctionName");
	writer.String(typeid(*m_pOnClickFunction).name());
	writer.EndObject();
}

void UIButtonComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_pOnClickFunction = dae::Factory<ButtonFunction>::GetInstance().Create(value["FunctionName"].GetString());

};

void UIButtonComponent::SetOnClickFunction(ButtonFunction* function)
{
	m_pOnClickFunction = function;
}

void UIButtonComponent::CallOnClickFunction()
{
	(*m_pOnClickFunction)();
}