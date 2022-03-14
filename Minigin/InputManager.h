#pragma once
#include "Singleton.h"
#include <map>

class Command;
namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		//todo: add the other buttons
	};
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

	class InputManager
	{
	public:
		void AddControllerMap(const ControllerButtonData& data, std::unique_ptr<Command>&& pCommand);

		virtual void ProcessInput();
		virtual bool IsDownThisFrame(ControllerButton button) const = 0;
		virtual bool IsUpThisFrame(ControllerButton button) const = 0;
		virtual bool IsPressed(ControllerButton button) const = 0;
	private:
		using ControllerCommandMap = std::map<ControllerButtonData, std::unique_ptr<Command>>;
		ControllerCommandMap m_ControllerMap{};
	};
}
