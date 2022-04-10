#pragma once
#include "Singleton.h"
#include <imgui.h>
#include <imgui_plot.h>
#include <box2d.h>

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const SDL_FRect& srcRect, const SDL_FRect& dstRect, bool flipHorizontal = false) const;
		void RenderTexture(const Texture2D& texture, const SDL_FRect& srcRect, float x, float y, float width, float height, bool flipHorizontal = false) const;

		void RenderBox(float x, float y, float width, float height) const;
		void RenderPolygon(glm::vec2* points, const int count, glm::vec4 color = glm::vec4{0.f, 255.f, 0.f, 255.f});
		void RenderPolygon(b2Vec2* points, const int count, glm::vec4 color = glm::vec4{ 0.f, 255.f, 0.f, 255.f });

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	private:
	};
}

