#include "MiniginPCH.h"
#include "imgui_helpers.h"
#include "ResourceManager.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

bool ImGui::InputValue(const std::string& key, int& value)
{
	return ImGui::InputInt(key.c_str(), &value);
}

bool ImGui::InputValue(const std::string& key, char& value)
{
	return ImGui::InputText(key.c_str(), &value, 1);
}

bool ImGui::InputValue(const std::string& key, float& value)
{
	return ImGui::InputFloat(key.c_str(), &value);
}

bool ImGui::InputValue(const std::string& key, double& value)
{
	return ImGui::InputDouble(key.c_str(), &value);
}

bool ImGui::InputValue(const std::string& key, std::string& value)
{
	return ImGui::InputText(key.c_str(), &value);
}

bool ImGui::InputValue(const std::string& key, glm::vec2& value)
{
	return ImGui::InputFloat2(key.c_str(), &value.x);
}

bool ImGui::InputValue(const std::string& key, b2Vec2& value)
{
	return ImGui::InputFloat2(key.c_str(), &value.x);
}

bool ImGui::InputValue(const std::string& key, glm::vec3& value)
{
	return ImGui::InputFloat3(key.c_str(), &value.x);
}

bool ImGui::InputValue(const std::string& key, glm::vec4& value)
{
	return ImGui::InputFloat4(key.c_str(), &value.x);
}

bool ImGui::InputValue(const std::string& key, SDL_FRect& value)
{
	return ImGui::InputFloat4(key.c_str(), &value.x);
}

// https://github.com/ocornut/imgui/issues/1658
bool ImGui::InputValue(const std::string& key, dae::Texture2D* value)
{
	auto list = dae::ResourceManager::GetInstance().GetTextureNames();

	static size_t currentItem = 0;
	//static const char* currentItem = list[0].c_str();

	if(ImGui::BeginCombo(key.c_str(), list[currentItem].c_str()))
	{
		for (size_t i = 0; i < list.size(); ++i)
		{
			bool isSelected = (currentItem == i);
			if (ImGui::Selectable(list[i].c_str(), isSelected))
			{
				currentItem = i;
				value = dae::ResourceManager::GetInstance().LoadTexture(list[i]);
				ImGui::EndCombo();
				return true;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	return false;
}
