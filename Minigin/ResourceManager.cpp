#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

void dae::ResourceManager::CleanUp()
{
	for (auto& pair : m_pTextures)
	{
		delete pair.second;
	}
	m_pTextures.clear();


	for (auto& pair : m_pFonts)
	{
		delete pair.second;
	}
	m_pFonts.clear();
}

dae::Texture2D* dae::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_DataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	m_pTextures[file] = new Texture2D(texture, file);

	return m_pTextures[file];
}

dae::Font* dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	m_pFonts[file] = new Font(m_DataPath + file, size);
	return m_pFonts[file];
}
