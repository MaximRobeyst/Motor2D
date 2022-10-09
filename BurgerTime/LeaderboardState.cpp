#include "LeaderboardState.h"
#include <SceneManager.h>
#include <InputManager.h>

#include <Scene.h>
#include <GameObject.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <TextObject.h>
#include <ResourceManager.h>
#include "MenuCommands.h"

#include "LeaderboardComponent.h"

LeaderboardState::LeaderboardState(int score)
	: m_CurrentScore{score}
{
}

void LeaderboardState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Leaderboard");
	auto& input = dae::InputManager::GetInstance();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	
	auto pTitle = new dae::GameObject("Title");
	pTitle->AddComponent(new dae::TransformComponent(pTitle, glm::vec3(100.f, 20.f, 0.f), glm::vec2{2.f, 2.f}));
	pTitle->AddComponent(new dae::SpriteRendererComponent(pTitle, "logo.png"));
	pTitle->AddComponent(new dae::TextComponent(pTitle, "Leaderboard", font));
	auto pLeaderboardComponent = new LeaderboardComponent(pTitle, m_CurrentScore);
	pTitle->AddComponent(pLeaderboardComponent);
	scene.AddGameObject(pTitle);
	
	for (int i = 0; i < 5; ++i)
	{
		auto go = new dae::GameObject("player " + std::to_string(i));
		go->AddComponent(new dae::TransformComponent(go, glm::vec3(100.f, 100.f + (25.f * i), 0.f)));
		go->AddComponent(new dae::SpriteRendererComponent(go, "logo.png"));
		go->AddComponent(new dae::TextComponent(go, "Player: " + std::to_string(i * 100), font));
		scene.AddGameObject(go);
		go->SetParent(pTitle);
	}
	
	scene.Start();
	
	auto keyboard = input.GetKeyboard();
	for (char i = 'a'; i < 'z'; ++i)
	{
		keyboard->AddKeyboardMapping(
			KeyboardKeyData{ i, KeyState::JustUp},
			new AddLetterCommand(pLeaderboardComponent, i)
		);
	}
	
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_BACKSPACE, KeyState::JustUp },
		new RemoveLetterCommand(pLeaderboardComponent)
	);
	
	scene.Start();

}

void LeaderboardState::OnExit()
{
	dae::SceneManager::GetInstance().GetScene("Leaderboard")->Serialize();
	dae::SceneManager::GetInstance().RemoveScene("Leaderboard");
}
