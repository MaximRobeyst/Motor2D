#include "MiniginPCH.h"
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

glm::vec2 dae::Texture2D::GetSize() const
{
	SDL_Point size;
	SDL_QueryTexture(m_pTexture, NULL, NULL, &size.x, &size.y);

	return glm::vec2(size.x, size.y);
}

std::string dae::Texture2D::GetPath() const
{
	return m_Path;
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture, const std::string& path)
	: m_pTexture{texture}
	, m_Path{path}
{
}
