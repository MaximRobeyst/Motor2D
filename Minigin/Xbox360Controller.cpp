#include "MiniginPCH.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Xbox360Controller.h"
#include <Xinput.h>
#include "Command.h"

#pragma comment(lib, "xinput.lib")

class dae::Xbox360Controller::Xbox360ControllerImpl
{
public:
	Xbox360ControllerImpl(int controllerIndex);
	~Xbox360ControllerImpl() = default;

	void Update();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame;
	WORD m_ButtonsReleasedThisFrame;

	int m_ControllerIndex;
};


dae::Xbox360Controller::Xbox360ControllerImpl::Xbox360ControllerImpl(int controllerIndex) : m_ControllerIndex{ controllerIndex }
{
	ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
}

void dae::Xbox360Controller::Xbox360ControllerImpl::Update()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Xbox360Controller::Xbox360ControllerImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool dae::Xbox360Controller::Xbox360ControllerImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool dae::Xbox360Controller::Xbox360ControllerImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

dae::Xbox360Controller::Xbox360Controller(int controllerIndex)
	: m_pImpl{ std::make_unique<Xbox360ControllerImpl>(controllerIndex) }
{
}

dae::Xbox360Controller::~Xbox360Controller() = default;

void dae::Xbox360Controller::ProcessInput()
{
	m_pImpl->Update();

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

bool dae::Xbox360Controller::IsDownThisFrame(ControllerButton button) const
{
	return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::Xbox360Controller::IsUpThisFrame(ControllerButton button) const
{
	return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::Xbox360Controller::IsPressed(ControllerButton button) const
{
	return m_pImpl->IsPressed(static_cast<unsigned int>(button));
}

void dae::Xbox360Controller::AddControllerMapping(const ControllerButtonData& controllerData, std::unique_ptr<Command>&& pCommand)
{
	m_ControllerMap[controllerData] = std::move(pCommand);
}
