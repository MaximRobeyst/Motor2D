#pragma once

enum class ControllerButton;
enum class ButtonState
{
	Hold,
	Down,
	Up,
};

struct ControllerButtonData
{
	ControllerButton controllerButton = {};
	ButtonState buttonState = {};
};

inline bool operator< (const ControllerButtonData& lhs, const ControllerButtonData& rhs)
{
	return lhs.controllerButton < rhs.controllerButton;
}

class Controller
{
public:
	Controller(int controllerIndex);

private:

};

