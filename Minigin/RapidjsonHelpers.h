#pragma once
#include <rapidjson.h>
#include <PrettyWriter.h>
#include <document.h>

#include <box2d.h>
#include <SDL.h>

namespace dae
{
	class GameObject;
	class Component;
	class Texture2D;
}

namespace rapidjson
{
	// Serialize 

	// Base values
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, int value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, char value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, float value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, double value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, std::string value);

	// Vectors & shapes
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec2& value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const b2Vec2& value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec3& value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec4& value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const SDL_FRect& value);

	// Engine Types
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::GameObject* value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::Component* value);
	void SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::Texture2D* value);


	// Deserialize
	// Base values
	void DeserializeValue(rapidjson::Value& value, const std::string& key, int& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, char& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, float& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, double& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, std::string& extract);

	// Vectors & shapes
	void DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec2& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, b2Vec2& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec3& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec4& extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, SDL_FRect& extract);


	// Engine Types
	void DeserializeValue(rapidjson::Value& value, const std::string& key, dae::GameObject** extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, dae::Component** extract);
	void DeserializeValue(rapidjson::Value& value, const std::string& key, dae::Texture2D** extract);

}


