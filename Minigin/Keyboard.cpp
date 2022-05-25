#include "MiniginPCH.h"
#include "Keyboard.h"

void dae::Keyboard::ClearInput()
{
    for (auto& keyboardCommand : m_KeyboardMap)
    {
        delete keyboardCommand.second;
    }

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
    if (m_KeyboardMap.size() == 0) return;

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

void dae::Keyboard::AddKeyboardMapping(const KeyboardKeyData& keyData, Command* pCommand)
{
    if (m_KeyboardMap[keyData] != nullptr)
        delete m_KeyboardMap[keyData];

    m_KeyboardMap[keyData] = pCommand;
}

void dae::Keyboard::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();

    for (auto& p : m_KeyboardMap)
    {
        std::cout << std::to_string( p.first.key) << std::endl;

        writer.StartObject();
        writer.Key("Key");
        writer.Int(p.first.key);
        writer.Key("State");
        writer.Int( static_cast<int>(p.first.state));
        writer.Key("Command");
        p.second->Serialize(writer);
        writer.EndObject();

    }

    writer.EndArray();
}

void dae::Keyboard::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
    ClearInput();
    for (auto iter = value.Begin(); iter != value.End(); ++iter)
    {
        auto& key = *iter;
        std::cout << "Key: " << key["Key"].GetInt() << std::endl;
        std::cout << "State: " << key["State"].GetInt() << std::endl;
        std::cout << "Command: " << key["Command"]["Name"].GetString() << std::endl;

        KeyboardKeyData newCommand{ key["Key"].GetInt(), static_cast<KeyState>(key["State"].GetInt()) };

        m_KeyboardMap[newCommand] = Factory<Command>::GetInstance().Create(key["Command"]["Name"].GetString());

        m_KeyboardMap[newCommand]->Deserialize(key["Command"], pScene);
   }
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
