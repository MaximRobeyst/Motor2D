#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"

void dae::InputManager::AddControllerMap(const ControllerButtonData& data, std::unique_ptr<Command>&& pCommand)
{
	m_ControllerMap[data] = std::move(pCommand);
}

//bool dae::InputManager::ProcessInput()
//{
//	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
//	XInputGetState(0, &m_CurrentState);
//
//	SDL_Event e;
//	while (SDL_PollEvent(&e)) {
//		if (e.type == SDL_QUIT) {
//			return false;
//		}
//		if (e.type == SDL_KEYDOWN) {
//			
//		}
//		if (e.type == SDL_MOUSEBUTTONDOWN) {
//			
//		}
//	}
//
//	return true;
//}

void dae::InputManager::ProcessInput()
{
	for (auto it = m_ControllerMap.begin(); it != m_ControllerMap.end(); ++it)
	{
		if (it->first.buttonState == ButtonState::Hold && IsPressed(it->first.controllerButton))
			it->second->Execute();
		if (it->first.buttonState == ButtonState::Down && IsDownThisFrame(it->first.controllerButton))
			it->second->Execute();
		if (it->first.buttonState == ButtonState::Up && IsUpThisFrame(it->first.controllerButton))
			it->second->Execute();
	}
}

//bool dae::InputManager::IsPressed(ControllerButton button) const
//{
//	switch (button)
//	{
//	case ControllerButton::ButtonA:
//		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
//	case ControllerButton::ButtonB:
//		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
//	case ControllerButton::ButtonX:
//		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
//	case ControllerButton::ButtonY:
//		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
//	default: return false;
//	}
//}

