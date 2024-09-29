#include "MainMenuScene.h"
#include "OptionsScene.h"
#include "CreditsScene.h"
#include "Engine.h"
#include "LaunchFunction.h"
#include "SelectCapacityScene.h"
#include "WindowModule.h"
#include <iostream>



MainMenuScene::MainMenuScene() : Scene("MainMenuScene", [] {})
{
	Engine::SetMusicState(true);
	/*if (!music.openFromFile("Game_files/Assets/Sons/musique_level_3.ogg")) {
		std::cout << "La musique ne charge pas" << std::endl;
	}

	music.setLoop(true);
	music.setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
	music.play();*/


	SetTexture("background", "Game_files/Assets/background/main_menu_background.png");
	SetBackground("background");

	SetTexture("texture_button_Play", "Game_files/Assets/button/buttonPlay.png");
	SetTexture("texture_button_Options", "Game_files/Assets/button/buttonOptions.png");
	SetTexture("texture_button_Credits", "Game_files/Assets/button/buttonCredits.png");
	SetTexture("texture_button_Quit", "Game_files/Assets/button/buttonQuit.png");

	sf::Vector2u window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow()->getSize();

	GameObject* play_game_button = CreateSpriteButton_forMainMenu(ButtonType, "main_menu_play_button", Maths::Vector2f((window_size.x / 10) * 2, (window_size.y - window_size.y * 5 / 6)), Maths::Vector2f(window_size.x / 15 * 3, (((window_size.x / 15) * 3) * 168 / 448)), [this] {LaunchFunction::LaunchScene<SelectCapacityScene>();  }, nullptr, "texture_button_Play", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));
	GameObject* option_game_button = CreateSpriteButton_forMainMenu(ButtonType, "main_menu_option_button", Maths::Vector2f((window_size.x / 10) * 2, (window_size.y - window_size.y * 4 / 6)), Maths::Vector2f(window_size.x / 15 * 3, (((window_size.x / 15) * 3) * 168 / 448)), [this] {LaunchFunction::LaunchScene<OptionsScene>(); }, nullptr, "texture_button_Options", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));
	GameObject* credit_game_button = CreateSpriteButton_forMainMenu(ButtonType, "main_menu_credits_button", Maths::Vector2f((window_size.x / 10) * 2, (window_size.y - window_size.y * 3 / 6)), Maths::Vector2f(window_size.x / 15 * 3, (((window_size.x / 15) * 3) * 168 / 448)), [this] { LaunchFunction::LaunchScene<CreditsScene>(); }, nullptr, "texture_button_Credits", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));
	GameObject* quit_game_button = CreateSpriteButton_forMainMenu(ButtonType, "main_menu_quit_button", Maths::Vector2f((window_size.x / 10) * 2, (window_size.y - window_size.y * 2 / 6)), Maths::Vector2f(window_size.x / 15 * 3, (((window_size.x / 15) * 3) * 168 / 448)), [this] {Engine::GetInstance()->Quit(); }, nullptr, "texture_button_Quit", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));

	if (!play_game_button || !option_game_button || !credit_game_button || !quit_game_button)
	{
		music.stop();
	}
}
