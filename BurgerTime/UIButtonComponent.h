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

class UIButtonComponent final : public dae::Component
{
public:
	UIButtonComponent() = default;
	UIButtonComponent(dae::GameObject* pGameobject, glm::vec4 color = glm::vec4{1,0,0,1});
	virtual ~UIButtonComponent() = default;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;

	void SetOnClickFunction(std::function<void()> function);
	void CallOnClickFunction();

private:
	std::function<void()> m_OnClickFunction;
};

