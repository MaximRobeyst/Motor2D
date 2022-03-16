#pragma once
#include <map>

class Command;

enum class KeyState
{
	Hold,
	Down,
	Up
};

struct KeyboardKeyData
{
	int key{};
	KeyState state{};
};

namespace dae
{
	class Keyboard final
	{
	public:
		Keyboard();
		virtual ~Keyboard();

		void ProcessInput();
		bool IsDownThisFrame(int key) const;
		bool IsUpThisFrame(int key) const;
		bool IsPressed(int key) const;

		void AddKeyboardMapping(const KeyboardKeyData& controllerData, std::unique_ptr<Command>&& pCommand);

	private:
		using KayboardCommandMap = std::map<KeyboardKeyData, std::unique_ptr<Command>>;
		KayboardCommandMap m_KeyboardMap{};

		Uint8* m_pPreviousKeyboardState;
		Uint8* m_pCurrKeyboardState;

		WORD m_ButtonsPressedThisFrame;
		WORD m_ButtonsReleasedThisFrame;
	};
}

