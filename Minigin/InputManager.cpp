#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include "Xbox360Controller.h"
#include "Keyboard.h"

#include <backends/imgui_impl_sdl.h>

bool dae::InputManager::ProcessInput()
{
	// https://gamedev.stackexchange.com/questions/190070/how-do-i-implement-an-input-wrapper-like-unitys-in-sdl2
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		m_pKeyboard->ProcessInput(&e);
		ImGui_ImplSDL2_ProcessEvent(&e);
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
