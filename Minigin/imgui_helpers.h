#pragma once
#include <box2d.h>
#include <SDL.h>

namespace dae
{
	class GameObject;
	class Component;
	class Texture2D;
}

namespace ImGui
{
	// IMGUI helpers
	// Base values
	bool InputValue(const std::string& key, int& value);
	bool InputValue(const std::string& key, char& value);
	bool InputValue(const std::string& key, float& value);
	bool InputValue(const std::string& key, double& value);
	bool InputValue(const std::string& key, std::string& value);

	// Vectors & shapes
	bool InputValue(const std::string& key, glm::vec2& value);
	bool InputValue(const std::string& key, b2Vec2& value);
	bool InputValue(const std::string& key, glm::vec3& value);
	bool InputValue(const std::string& key, glm::vec4& value);
	bool InputValue(const std::string& key, SDL_FRect& value);

	// Engine Types
	//void InputValue(const std::string& key, dae::GameObject* value);
	//void InputValue(const std::string& key, dae::Component* value);
	bool InputValue(const std::string& key, dae::Texture2D* value);

}

