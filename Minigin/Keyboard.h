#pragma once
#include <map>
#include "Command.h"
#include <SDL.h>

enum class KeyboardAndMouseButton
{

};

enum class KeyState
{
	JustUp,
	Up,
	Down,
	Hold,
};

struct KeyboardKeyData
{
	int key{};
	KeyState state{};
};

inline bool operator< (const KeyboardKeyData& lhs, const KeyboardKeyData& rhs)
{
	return lhs.key < rhs.key;
}

namespace dae
{
	class Keyboard final
	{
	public:
		Keyboard()  = default;
		virtual ~Keyboard() = default;

		void ClearInput();

		void ProcessInput(const SDL_Event* event);
		bool IsDownThisFrame(int key);
		bool IsUpThisFrame(int key);
		bool IsPressed(int key);

		void AddKeyboardMapping(const KeyboardKeyData& controllerData, std::unique_ptr<Command>&& pCommand);

	private:
		void UpdateKeys();

		using KayboardCommandMap = std::map<KeyboardKeyData, std::unique_ptr<Command>>;
		KayboardCommandMap m_KeyboardMap{};

		using keyIndex = int;
		std::map<keyIndex, KeyState> m_KeyStates;
		std::map<keyIndex, KeyState> m_MouseStates;
	};
}

