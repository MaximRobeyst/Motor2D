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

void UIButtonComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void UIButtonComponent::SetOnClickFunction(std::function<void()> function)
{
	m_OnClickFunction = function;
}

void UIButtonComponent::CallOnClickFunction()
{
	m_OnClickFunction();
}