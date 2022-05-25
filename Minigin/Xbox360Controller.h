#pragma once
#include "InputManager.h"
#include <memory>

class Command;

namespace dae
{
	enum class ControllerButton
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000
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

	class Xbox360Controller final
	{
		class Xbox360ControllerImpl;
		std::unique_ptr<Xbox360ControllerImpl> m_pImpl;
	public:

		Xbox360Controller(int controllerIndex);
		virtual ~Xbox360Controller();

		Xbox360Controller(const Xbox360Controller&) = delete;
		Xbox360Controller(const Xbox360Controller&&) = delete;
		Xbox360Controller& operator=(const Xbox360Controller&) = delete;
		Xbox360Controller& operator= (const Xbox360Controller&&) = delete;

		void ClearInputs();

		void ProcessInput();
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const ;
		bool IsPressed(ControllerButton button) const;

		void AddControllerMapping(const ControllerButtonData& controllerData, Command* pCommand);
		// RemoveControllerMapping

	private:
		using ControllerCommandMap = std::map<ControllerButtonData, Command*>;
		ControllerCommandMap m_ControllerMap{};
	};
}

