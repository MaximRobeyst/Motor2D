#pragma once
#include "Singleton.h"

#include <unordered_map>

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		void CleanUp();
		Texture2D* LoadTexture(const std::string& file) const;
		Font* LoadFont(const std::string& file, unsigned int size) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::unordered_map<std::string, Texture2D*> m_pTextures;
		std::unordered_map<std::string, Font*> m_pFonts;
	};
}
