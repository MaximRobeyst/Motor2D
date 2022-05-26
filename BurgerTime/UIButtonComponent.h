#pragma once
#include <Component.h>
#include <glm/glm.hpp>

#include <functional>

namespace dae
{
	class TransformComponent;
	class SpriteRendererComponent;
	class TextComponent;
}


struct ButtonFunction
{
	virtual void operator()() = 0;
};

class UIButtonComponent final : public dae::Component
{
public:
	UIButtonComponent() = default;
	UIButtonComponent(dae::GameObject* pGameobject, glm::vec4 color = glm::vec4{1,0,0,1});
	virtual ~UIButtonComponent();

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	virtual void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& /*value*/);

	void SetOnClickFunction(ButtonFunction* function);
	void CallOnClickFunction();

private:
	ButtonFunction* m_pOnClickFunction;
};

