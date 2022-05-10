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
	UIButtonComponent(dae::GameObject* pGameobject, glm::vec4 color = glm::vec4{1,0,0,1});
	virtual ~UIButtonComponent() = default;

	void Start() override;
	void Update() override;
	void Render() const override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;

	void SetOnClickFunction(std::function<void()> function);

private:
	std::function<void()> m_OnClickFunction;

	bool OverLapping(const glm::vec2& mousePosition) const;

	dae::TransformComponent* m_pTransformComponent;
	dae::SpriteRendererComponent* m_pSpriteComponent;
	dae::TextComponent* m_pTextComponent{};
	glm::vec2 m_BoundsMin{};
	glm::vec2 m_BoundsMax{};
	glm::vec4 m_NewColor{1, 0, 0, 1.f};

};

