#pragma once
#include "Component.h"
#include "Observer.h"
#include "TextObject.h"

class Subject;
class ScoreDisplayComponent : public dae::Component, public Observer
{
public:
	ScoreDisplayComponent();
	ScoreDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText = "");

	void Start() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& /*value*/) override;

	void Notify(const dae::GameObject& gameObject, const Event& action);
	void ChangeText(int number);

	static int GetScore();

	std::unique_ptr<Subject>& GetSubject();
private:
	dae::TextComponent* m_pTextComponent;
	std::string m_ExtraDisplayText;

	static int m_Score;

	std::unique_ptr<Subject> m_pSubject;
};

