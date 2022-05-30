#pragma once
#include "Singleton.h"
#include "Keyboard.h"
#include <map>

namespace dae
{
	class InputManager;

	class Xbox360Controller;
	class AxisManager
	{
	public:
		AxisManager() = default;
		virtual ~AxisManager() = default;

		virtual int GetAxisValue() = 0;

		virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
		virtual void Deserialize(rapidjson::Value& value, InputManager* pInputManager) = 0;
	};


	class KeyboardAxis final : public AxisManager
	{
	public:
		KeyboardAxis() = default;
		KeyboardAxis(int positive, int negative, std::shared_ptr<dae::Keyboard> pInputManager);

		virtual int GetAxisValue() override;

		virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
		virtual void Deserialize(rapidjson::Value& value, InputManager* pInputManager) override;
	private:
		int m_KeyboardKeyPositive;
		int m_KeyboardKeyNegative;
		std::shared_ptr<dae::Keyboard> m_Keyboard;

	};

	enum class ControllerButton;
	class ControllerAxis final : public AxisManager
	{
	public:
		ControllerAxis() = default;
		ControllerAxis(ControllerButton positive, ControllerButton negative, std::shared_ptr<dae::Xbox360Controller> controller);

		virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
		virtual void Deserialize(rapidjson::Value& value, InputManager* pScene) override;

		virtual int GetAxisValue() override;
	private:
		ControllerButton m_ControllerButtonPositive;
		ControllerButton m_ControllerButtonNegative;
		std::shared_ptr<dae::Xbox360Controller> m_Controller;

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
		void Deserialize(rapidjson::Value& value, dae::Scene* pScene);

		glm::vec2 GetMousePosition() const;

		void AddAxis(const std::string& name, AxisManager* axis);
		AxisManager* GetAxis(const std::string& name);

	private:
		std::shared_ptr<dae::Keyboard> m_pKeyboard{ std::make_shared<dae::Keyboard>() };
		std::vector<std::shared_ptr<dae::Xbox360Controller>> m_pXbox360Controllers{};
		std::map<std::string, AxisManager*> m_Axeses{};
	};


}
