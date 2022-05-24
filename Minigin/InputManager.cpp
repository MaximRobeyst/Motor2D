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

dae::ControllerAxis::ControllerAxis(ControllerButton positive, ControllerButton negative, std::shared_ptr<dae::Xbox360Controller> controller)
	: m_ControllerButtonPositive{ positive }
	, m_ControllerButtonNegative{ negative }
	, m_Contrller{ controller }
{
}

int dae::ControllerAxis::GetAxisValue()
{
	return (int)(m_Contrller->IsPressed(m_ControllerButtonPositive) - m_Contrller->IsPressed(m_ControllerButtonNegative));
}
