#include "MiniginPCH.h"
#include "RapidjsonHelpers.h"

#include "GameObject.h"
#include "Component.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "ResourceManager.h"

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, int value)
{
	writer.Key(key.c_str());
	writer.Int(value);
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, char value)
{
	writer.Key(key.c_str());
	writer.Int(static_cast<int>(value));
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, float value)
{
	writer.Key(key.c_str());
	writer.Double(static_cast<double>(value));
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, double value)
{
	writer.Key(key.c_str());
	writer.Double(value);
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, std::string value)
{
	writer.Key(key.c_str());
	writer.String(value.c_str());
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec2& value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	writer.Key("X");
	writer.Double(static_cast<double>(value.x));
	writer.Key("Y");
	writer.Double(static_cast<double>(value.y));
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const b2Vec2& value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	writer.Key("X");
	writer.Double(static_cast<double>(value.x));
	writer.Key("Y");
	writer.Double(static_cast<double>(value.y));
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec3& value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	writer.Key("X");
	writer.Double(static_cast<double>(value.x));
	writer.Key("Y");
	writer.Double(static_cast<double>(value.y));
	writer.Key("Z");
	writer.Double(static_cast<double>(value.z));
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const glm::vec4& value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	writer.Key("X");
	writer.Double(static_cast<double>(value.x));
	writer.Key("Y");
	writer.Double(static_cast<double>(value.y));
	writer.Key("Z");
	writer.Double(static_cast<double>(value.z));
	writer.Key("W");
	writer.Double(static_cast<double>(value.w));
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, const SDL_FRect& rect)
{
	writer.Key(key.c_str());
	writer.StartObject();
	SerializeValue(writer, "X", rect.x);
	SerializeValue(writer, "Y", rect.y);
	SerializeValue(writer, "W", rect.w);
	SerializeValue(writer, "H", rect.h);
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::GameObject* value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	value->Serialize(writer);
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::Component* value)
{
	writer.Key(key.c_str());
	writer.StartObject();
	value->Serialize(writer);
	writer.EndObject();
}

void rapidjson::SerializeValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& key, dae::Texture2D* value)
{
	writer.Key(key.c_str());
	writer.String(value->GetPath().c_str());
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, int& extract)
{
	extract = value[key.c_str()].GetInt();
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, char& extract)
{
	extract = static_cast<char>(value[key.c_str()].GetInt());
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, float& extract)
{
	extract = static_cast<float>(value[key.c_str()].GetDouble());
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, double& extract)
{
	extract = value[key.c_str()].GetDouble();
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, std::string& extract)
{
	extract = value[key.c_str()].GetString();
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec2& extract)
{
	extract = glm::vec2
	{
		static_cast<float>(value[key.c_str()]["X"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Y"].GetDouble())
	};
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, b2Vec2& extract)
{
	extract = b2Vec2
	{
		static_cast<float>(value[key.c_str()]["X"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Y"].GetDouble())
	};
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec3& extract)
{
	extract = glm::vec3
	{
		static_cast<float>(value[key.c_str()]["X"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Y"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Z"].GetDouble())
	};
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, glm::vec4& extract)
{
	extract = glm::vec4
	{
		static_cast<float>(value[key.c_str()]["X"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Y"].GetDouble()),
		static_cast<float>(value[key.c_str()]["Z"].GetDouble()),
		static_cast<float>(value[key.c_str()]["W"].GetDouble()),
	};
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, SDL_FRect& extract)
{
	extract = SDL_FRect{
		   static_cast<float>(value[key.c_str()]["X"].GetDouble()),
		   static_cast<float>(value[key.c_str()]["Y"].GetDouble()),
		   static_cast<float>(value[key.c_str()]["W"].GetDouble()),
		   static_cast<float>(value[key.c_str()]["H"].GetDouble())
	};
}

void rapidjson::DeserializeValue(rapidjson::Value& value, const std::string& key, dae::Texture2D** extract)
{
	*extract = dae::ResourceManager::GetInstance().LoadTexture(value[key.c_str()].GetString());
}
