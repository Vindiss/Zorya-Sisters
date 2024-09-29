#include "MapScene.h"

#include "LaunchFunction.h"
#include "SelectCapacityScene.h"

MapScene::MapScene() : Scene("MapScene", [] {})
{
	SetTexture("texture_return_button", "Game_files/Assets/button/return_button.png");
	SetTexture("background", "Game_files/Assets/background/Level3.designMap.png");
	SetBackground("background");

	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();

	GameObject* return_button = CreateSpriteButton_forMainMenu(ButtonType, "return_button", Maths::Vector2f(window_size.x / 50, window_size.y - window_size.y / 50 - 144), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 161) / 144))), [this] {LaunchFunction::resumeGame(); }, nullptr, "texture_return_button", Maths::Vector2f(144, 161), Maths::Vector2f(0, 15));
}