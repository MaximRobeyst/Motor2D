#pragma once
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"

#include <memory>
#include <SDL.h>

namespace dae
{
	// Text component should not render

	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		//TextComponent(dae::GameObject* pGameObject, const std::string& text, const std::shared_ptr<Font>& font);
		explicit TextComponent(GameObject* pGameObject, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color = {255,255,255});
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent & other) = delete;
		TextComponent(TextComponent && other) = delete;
		TextComponent& operator=(const TextComponent & other) = delete;
		TextComponent& operator=(TextComponent && other) = delete;

		void Update() override;
		void Render() const override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);
		void SetColor(const SDL_Color& color);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		SDL_Color m_Color;
	};
}
