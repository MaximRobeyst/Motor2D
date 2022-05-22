#include "MiniginPCH.h"
#include "Keyboard.h"

void dae::Keyboard::ClearInput()
{
    m_KeyboardMap.clear();
}

void dae::Keyboard::ProcessInput(const SDL_Event* event)
{
    UpdateKeys();

    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
    {
        KeyState newState{};

        if (event->key.type == SDL_KEYDOWN)
            newState = KeyState::Down;
        else if (event->key.type == SDL_KEYUP)
            newState = KeyState::JustUp;

        m_KeyStates[event->key.keysym.sym] = newState;
    }
    for (auto it = m_KeyboardMap.begin(); it != m_KeyboardMap.end(); ++it)
    {
        if (it == m_KeyboardMap.end())
            break;

        if (it->first.state == KeyState::Hold && IsPressed(it->first.key))
            it->second->Execute();
        if (it->first.state == KeyState::Down && IsDownThisFrame(it->first.key))
            it->second->Execute();
        if (it->first.state == KeyState::JustUp && IsUpThisFrame(it->first.key))
            it->second->Execute();
    }

}

bool dae::Keyboard::IsDownThisFrame(int key)
{
    if (m_KeyStates.count(key) > 0)
    {
        if (m_KeyStates[key] == KeyState::Down)
            return true;
    }
    return false;
}

bool dae::Keyboard::IsUpThisFrame(int key)
{
    if (m_KeyStates.count(key) > 0)
    {
        if (m_KeyStates[key] == KeyState::JustUp)
            return true;
    }
    return false;
}

bool dae::Keyboard::IsPressed(int key)
{
    if (m_KeyStates.count(key) > 0)
    {
        if (m_KeyStates[key] == KeyState::Hold)
            return true;
    }
    return false;
}

void dae::Keyboard::AddKeyboardMapping(const KeyboardKeyData& keyData, std::unique_ptr<Command>&& pCommand)
{
    m_KeyboardMap[keyData] = std::move(pCommand);
}

void dae::Keyboard::UpdateKeys()
{
    for (auto& [keyIndex, state] : m_KeyStates)
    {
        if (state == KeyState::JustUp)
            state = KeyState::Up;
        else if (state == KeyState::Down)
            state = KeyState::Hold;
    }
}
