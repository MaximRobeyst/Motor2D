#include "MiniginPCH.h"
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

glm::vec2 dae::Texture2D::GetSize() const
{
	SDL_Point size;
	SDL_QueryTexture(m_Texture, NULL, NULL, &size.x, &size.y);

	return glm::vec2(size.x, size.y);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
