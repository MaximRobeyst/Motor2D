#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include "Xbox360Controller.h"
#include "Keyboard.h"

#include <backends/imgui_impl_sdl.h>

bool dae::InputManager::ProcessInput()
{
	auto& io = ImGui::GetIO();
	// https://gamedev.stackexchange.com/questions/190070/how-do-i-implement-an-input-wrapper-like-unitys-in-sdl2
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (io.WantCaptureKeyboard)
			continue;
		m_pKeyboard->ProcessInput(&e);
	}

	for (size_t i = 0; i < m_pXbox360Controllers.size(); i++)
	{
		m_pXbox360Controllers[i]->ProcessInput();
	}

	return true;
}

void dae::InputManager::AddController(std::shared_ptr<dae::Xbox360Controller>& controller)
{
	m_pXbox360Controllers.push_back(controller);
}

void dae::InputManager::ClearInputs()
{
	for (auto controller : m_pXbox360Controllers)
	{
		controller->ClearInputs();
	}

	for (auto axis : m_Axeses)
	{
		delete axis.second;
	}

	m_Axeses.clear();
	m_pKeyboard->ClearInput();
}

std::shared_ptr<dae::Xbox360Controller> dae::InputManager::GetController(int index)
{
	return m_pXbox360Controllers[index];
}

std::shared_ptr<dae::Keyboard> dae::InputManager::GetKeyboard() const
{
	return m_pKeyboard;
}

void dae::InputManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("ControllerCount");
	writer.Int(static_cast<int>(m_pXbox360Controllers.size()));

	writer.Key("Keyboard");
	m_pKeyboard->Serialize(writer);

	for (size_t i = 0; i < m_pXbox360Controllers.size(); ++i)
	{
		writer.Key(("Controller " + std::to_string(i)).c_str());
		m_pXbox360Controllers[i]->Serialize(writer);
	}

	if (m_Axeses.size() <= 0)
	{
		writer.EndObject();
		return;
	}

	writer.Key("Axises");
	writer.StartArray();
	for (auto& axis : m_Axeses)
	{
		writer.StartObject();
		writer.Key("Name");
		writer.String(axis.first.c_str());

		writer.Key("Axis");
		axis.second->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();
}

void dae::InputManager::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	for (size_t i = 0; i < m_pXbox360Controllers.size(); ++i)
	{
		m_pXbox360Controllers[i]->ClearInputs();
	}
	m_pXbox360Controllers.clear();

	const size_t controllerCount = value["ControllerCount"].GetInt();
	m_pXbox360Controllers.reserve( controllerCount);
	m_pKeyboard->Deserialize(value["Keyboard"], pScene);
	for (size_t i = 0; i < controllerCount; ++i)
	{
		std::shared_ptr<Xbox360Controller> controller = std::make_shared<Xbox360Controller>(i);
		AddController(controller);
		controller->Deserialize(value[("Controller " + std::to_string(i)).c_str()], pScene);
	}

	for (auto axis : m_Axeses) delete axis.second;
	m_Axeses.clear();

	if (!value.HasMember("Axises")) return;
	for (auto iter = value["Axises"].Begin(); iter != value["Axises"].End(); ++iter)
	{
		auto& key = *iter;
		m_Axeses[key["Name"].GetString()] = dae::Factory<AxisManager>::GetInstance().Create(key["Axis"]["Name"].GetString());
		m_Axeses[key["Name"].GetString()]->Deserialize(key["Axis"], this);
	}
}

glm::vec2 dae::InputManager::GetMousePosition() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2{ (float)x, (float)y };
}

void dae::InputManager::AddAxis(const std::string& name, AxisManager* axis)
{
	if (m_Axeses[name] != nullptr) return;
	m_Axeses[name] = axis;
}

dae::AxisManager* dae::InputManager::GetAxis(const std::string& name)
{
	return m_Axeses[name];
}

dae::Creator<dae::AxisManager, dae::KeyboardAxis> g_KeyboardAxisCreator{};

dae::KeyboardAxis::KeyboardAxis(int positive, int negative, std::shared_ptr<dae::Keyboard> keyboard)
	: m_KeyboardKeyPositive{positive}
	, m_KeyboardKeyNegative{negative}
	, m_Keyboard{keyboard}
{
}

int dae::KeyboardAxis::GetAxisValue()
{
	return (int)(m_Keyboard->IsPressed(m_KeyboardKeyPositive) -m_Keyboard->IsPressed(m_KeyboardKeyNegative));
}

void dae::KeyboardAxis::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("PositiveKey");
	writer.Int(m_KeyboardKeyPositive);
	writer.Key("NegativeKey");
	writer.Int(m_KeyboardKeyNegative);
	writer.EndObject();
}

void dae::KeyboardAxis::Deserialize(rapidjson::Value& value, dae::InputManager* pInputManager)
{
	m_Keyboard = pInputManager->GetKeyboard();
	m_KeyboardKeyPositive = value["PositiveKey"].GetInt();
	m_KeyboardKeyNegative = value["NegativeKey"].GetInt();
}

dae::Creator<dae::AxisManager, dae::ControllerAxis> g_ControllerAxisCreator{};

dae::ControllerAxis::ControllerAxis(ControllerButton positive, ControllerButton negative, std::shared_ptr<dae::Xbox360Controller> controller)
	: m_ControllerButtonPositive{ positive }
	, m_ControllerButtonNegative{ negative }
	, m_Controller{ controller }
{
}

void dae::ControllerAxis::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("PositiveKey");
	writer.Int(static_cast<int>(m_ControllerButtonPositive));
	writer.Key("NegativeKey");
	writer.Int(static_cast<int>(m_ControllerButtonNegative));
	writer.Key("ControllerIndex");
	writer.Int(m_Controller->GetIndex());
	writer.EndObject();
}

void dae::ControllerAxis::Deserialize(rapidjson::Value& value, dae::InputManager* pScene)
{
	m_Controller = pScene->GetController(value["ControllerIndex"].GetInt());
	m_ControllerButtonPositive = static_cast<ControllerButton>(value["PositiveKey"].GetInt());
	m_ControllerButtonNegative = static_cast<ControllerButton>(value["NegativeKey"].GetInt());
}

int dae::ControllerAxis::GetAxisValue()
{
	return (int)(m_Controller->IsPressed(m_ControllerButtonPositive) - m_Controller->IsPressed(m_ControllerButtonNegative));
}
