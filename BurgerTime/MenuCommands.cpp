#include "MenuCommands.h"
#include "GameStateManager.h"
#include "Level1State.h"
#include "MenuCommands.h"
#include "MenuComponent.h"
#include "MainMenuState.h"
#include "SceneManager.h"
#include "Scene.h"


dae::Creator<Command, SwitchMenuStateCommand> g_SwitchMenuCommandCreator;

void SwitchMenuStateCommand::Execute()
{
	GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
}


dae::Creator<Command, PressButtonCommand> g_PressButtonCommandCreator;

PressButtonCommand::PressButtonCommand(MenuComponent* pMenuComp)
	: m_pMenuComponent{pMenuComp}
{
}


void PressButtonCommand::Execute()
{
	m_pMenuComponent->PressButton();
}

void PressButtonCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("Menucomponent");
	writer.Int(m_pMenuComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void PressButtonCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_pMenuComponent = pScene->GetGameobjectFromId(value["Menucomponent"].GetInt())->GetComponent<MenuComponent>();
}

dae::Creator<Command, ChangePointerCommand> g_ChangePointerCommandCreator;

ChangePointerCommand::ChangePointerCommand(MenuComponent* pMenuComp, int change)
	: m_pMenuComponent{pMenuComp}
	, m_Change{change}
{
}

void ChangePointerCommand::Execute()
{
	m_pMenuComponent->SwitchSelection(m_Change);
}

void ChangePointerCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("Change");
	writer.Int(m_Change);

	writer.Key("Menucomponent");
	writer.Int(m_pMenuComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void ChangePointerCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_Change = value["Change"].GetInt();
	m_pMenuComponent = pScene->GetGameobjectFromId(value["Menucomponent"].GetInt())->GetComponent<MenuComponent>();

}
