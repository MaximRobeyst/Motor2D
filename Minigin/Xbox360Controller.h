#pragma once
#include "InputManager.h"
#include <memory>

namespace dae
{
	class Xbox360Controller final : public dae::InputManager
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

		void ProcessInput();
		bool IsDownThisFrame(ControllerButton button) const override;
		bool IsUpThisFrame(ControllerButton button) const override;
		bool IsPressed(ControllerButton button) const override;

	private:

	};
}

