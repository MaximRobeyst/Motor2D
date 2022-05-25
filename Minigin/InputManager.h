#pragma once
#include "Singleton.h"
#include "Keyboard.h"
#include <map>

namespace dae
{

	class Xbox360Controller;
	class AxisManager
	{
	public:
		AxisManager() = default;
		virtual ~AxisManager() = default;

		virtual int GetAxisValue() = 0;
	};


	class KeyboardAxis final : public AxisManager
	{
	public:
		KeyboardAxis(int positive, int negative, std::shared_ptr<dae::Keyboard> keyboard);

		virtual int GetAxisValue() override;
	private:
		int m_KeyboardKeyPositive;
		int m_KeyboardKeyNegative;
		std::shared_ptr<dae::Keyboard> m_Keyboard;

	};

	enum class ControllerButton;
	class ControllerAxis final : public AxisManager
	{
	public:
		ControllerAxis(ControllerButton positive, ControllerButton negative, std::shared_ptr<dae::Xbox360Controller> controller);

		virtual int GetAxisValue() override;
	private:
		ControllerButton m_ControllerButtonPositive;
		ControllerButton m_ControllerButtonNegative;
		std::shared_ptr<dae::Xbox360Controller> m_Contrller;

	};

	class InputManager : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddController(std::shared_ptr<dae::Xbox360Controller>& controller);
		void ClearInputs();

		std::shared_ptr<dae::Xbox360Controller> GetController(int index);
		std::shared_ptr<dae::Keyboard> GetKeyboard() const;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer);
		void Deserialize(rapidjson::Value& value);

		glm::vec2 GetMousePosition() const;

		void AddAxis(const std::string& name, AxisManager* axis);
		AxisManager* GetAxis(const std::string& name);

	private:
		std::shared_ptr<dae::Keyboard> m_pKeyboard{ std::make_shared<dae::Keyboard>() };
		std::vector<std::shared_ptr<dae::Xbox360Controller>> m_pXbox360Controllers{};
		std::map<std::string, AxisManager*> m_Axeses{};
	};


}
