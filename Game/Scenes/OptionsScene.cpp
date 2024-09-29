#include "OptionsScene.h"

#include <fstream>
#include <iostream>
#include "Engine.h"
#include "LaunchFunction.h"
#include "MainMenuScene.h"
#include "SceneModule.h"
#include "WindowModule.h"
#include "InputModule.h"

OptionsScene::OptionsScene() : Scene("OptionsScene", [] {})
{
	SetTexture("background", "Game_files/Assets/background/main_menu_background.png");
	SetTexture("texture_return_button", "Game_files/Assets/button/return_button.png");
	SetBackground("background");
	SetFont("Game_files/Font/UkrainianPrincess.ttf");
	SetTexture("texture_sound_button", "Game_files/Assets/button/buttonSon.png");
	SetTexture("texture_controls_button", "Game_files/Assets/button/buttonControles.png");
	SetTexture("texture_jauge_slider", "Game_files/Assets/button/jauge_slider.png");
	SetTexture("texture_button_slider", "Game_files/Assets/button/buttonSlider.png");
	SetTexture("texture_entry_button", "Game_files/Assets/button/entry_button.png");

	sf::Vector2u window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow()->getSize();


	GameObject* sound_button = CreateSpriteButton_forMainMenu(ButtonType, "sound_button", Maths::Vector2f(window_size.x / 100 * 7.55208, window_size.y / 2 - ((((window_size.x / 100 * 15.4167) * 168) / 448) * 1.5)), Maths::Vector2f(window_size.x / 100 * 15.4167, ((((window_size.x / 100 * 15.4167) * 168) / 448))), [] {}, nullptr, "texture_sound_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));
	sound_button->GetComponent<Button>()->SetCallback(std::bind(&Button::SoundMenu, sound_button->GetComponent<Button>()));
	GameObject* controls_button = CreateSpriteButton_forMainMenu(ButtonType, "controls_button", Maths::Vector2f(window_size.x / 100 * 7.55208, window_size.y / 2 + (((window_size.x / 100 * 15.4167) * 168) / 448) * 0.5), Maths::Vector2f(window_size.x / 100 * 15.4167, ((((window_size.x / 100 * 15.4167) * 168) / 448))), [] {}, nullptr, "texture_controls_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24));
	controls_button->GetComponent<Button>()->SetCallback(std::bind(&Button::ControlsMenu, sound_button->GetComponent<Button>()));

	GameObject* return_button = CreateSpriteButton_forMainMenu(ButtonType, "return_button", Maths::Vector2f(window_size.x / 50, window_size.y - window_size.y / 50 - 144), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 161) / 144))), [this] { LaunchFunction::LaunchScene<MainMenuScene>(); SetSettingsOnFile(); }, nullptr, "texture_return_button", Maths::Vector2f(144, 161), Maths::Vector2f(0, 15));
}