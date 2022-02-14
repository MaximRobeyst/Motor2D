#pragma once
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	// Text component should not render

	class Font;
	class Texture2D;
	class GameObject;
	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(dae::GameObject* pGameObject, const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent & other) = delete;
		TextComponent(TextComponent && other) = delete;
		TextComponent& operator=(const TextComponent & other) = delete;
		TextComponent& operator=(TextComponent && other) = delete;

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
