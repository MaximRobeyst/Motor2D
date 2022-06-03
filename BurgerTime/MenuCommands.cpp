#include "MenuCommands.h"
#include "GameStateManager.h"
#include "Level1State.h"
#include "MenuCommands.h"
#include "MenuComponent.h"
#include "MainMenuState.h"
#include "SceneManager.h"
#include "Scene.h"

#include "LeaderboardComponent.h"


const dae::Creator<Command, SwitchMenuStateCommand> g_SwitchMenuCommandCreator;

void SwitchMenuStateCommand::Execute()
{
	GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
}


const dae::Creator<Command, PressButtonCommand> g_PressButtonCommandCreator;

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

const dae::Creator<Command, ChangePointerCommand> g_ChangePointerCommandCreator;

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

const dae::Creator<Command, AddLetterCommand> g_AddLetterCommand;

AddLetterCommand::AddLetterCommand(LeaderboardComponent* pLeaderboardComponent, char letter)
	: m_pLeaderboardComponent{ pLeaderboardComponent }
	, m_Letter{letter}
{
}

void AddLetterCommand::Execute()
{
	m_pLeaderboardComponent->AddLetter(m_Letter);
}

void AddLetterCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("Letter");
	writer.Int(m_Letter);

	writer.Key("LeaderboardComponent");
	writer.Int(m_pLeaderboardComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void AddLetterCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_Letter = static_cast<char>( value["Letter"].GetInt());
	m_pLeaderboardComponent = pScene->GetGameobjectFromId(value["LeaderboardComponent"].GetInt())->GetComponent<LeaderboardComponent>();
}

const dae::Creator<Command, RemoveLetterCommand> g_RemoveLetterCommand;

RemoveLetterCommand::RemoveLetterCommand(LeaderboardComponent* pLeaderboardComponent)
	: m_pLeaderboardComponent{pLeaderboardComponent}
{
}

void RemoveLetterCommand::Execute()
{
	m_pLeaderboardComponent->RemoveLetter();
}

void RemoveLetterCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("LeaderboardComponent");
	writer.Int(m_pLeaderboardComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void RemoveLetterCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_pLeaderboardComponent = pScene->GetGameobjectFromId(value["LeaderboardComponent"].GetInt())->GetComponent<LeaderboardComponent>();
}
