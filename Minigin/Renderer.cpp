#include "MiniginPCH.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <chrono>


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
#ifdef _DEBUG
	SceneManager::GetInstance().RenderGUI(m_Window);
#endif // _DEBUG
	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const SDL_FRect& srcRect, const SDL_FRect& dstRect, bool /*flipHorizontal*/) const
{

	SDL_Rect dst{ static_cast<int>(dstRect.x), static_cast<int>(dstRect.y),static_cast<int>(dstRect.w),static_cast<int>(dstRect.h) };
	SDL_Rect src{ static_cast<int>(srcRect.x), static_cast<int>(srcRect.y),static_cast<int>(srcRect.w),static_cast<int>(srcRect.h) };

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const SDL_FRect& srcRect, float x, float y, float width, float height, bool flipHorizontal) const
{
	SDL_RendererFlip flip;
	if (flipHorizontal)
		flip = SDL_FLIP_HORIZONTAL;
	else
		flip = SDL_FLIP_NONE;

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_Rect src{ static_cast<int>(srcRect.x), static_cast<int>(srcRect.y),static_cast<int>(srcRect.w),static_cast<int>(srcRect.h) };

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, 0.0, nullptr, flip);
}

void dae::Renderer::RenderBox(float x, float y, float width, float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(GetSDLRenderer(), &r, &g, &b, &a);

	SDL_SetRenderDrawColor(GetSDLRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawRect(GetSDLRenderer(), &dst);

	SDL_SetRenderDrawColor(GetSDLRenderer(), r, g, b, a);
}

void dae::Renderer::RenderPolygon(glm::vec2* points, const int count, glm::vec4 color)
{
	std::vector<SDL_FPoint> pointsSDL(count);
	for (int i = 0; i < count; ++i)
		pointsSDL[i] = SDL_FPoint{ points[i].x, points[i].y };

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(GetSDLRenderer(), &r, &g, &b, &a);
	SDL_SetRenderDrawColor(GetSDLRenderer(), (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, (Uint8)color.a);
	SDL_RenderDrawPointsF(GetSDLRenderer(), pointsSDL.data(), count);
	SDL_SetRenderDrawColor(GetSDLRenderer(), r, g, b, a);
}

void dae::Renderer::RenderPolygon(b2Vec2* points, const int count, glm::vec4 color)
{
	std::vector<SDL_FPoint> pointsSDL(count);
	for (int i = 0; i < count; ++i)
		pointsSDL[i] = SDL_FPoint{ points[i].x, points[i].y };

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(GetSDLRenderer(), &r, &g, &b, &a);
	SDL_SetRenderDrawColor(GetSDLRenderer(), (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, (Uint8)color.a);
	SDL_RenderDrawPointsF(GetSDLRenderer(), pointsSDL.data(), count);
	SDL_SetRenderDrawColor(GetSDLRenderer(), r, g, b, a);
}
