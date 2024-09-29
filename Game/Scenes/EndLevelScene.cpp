#include "EndLevelScene.h"

#include "LaunchFunction.h"
#include "MainMenuScene.h"

EndLevelScene::EndLevelScene() : Scene("EndLevelScene", [] {})
{
	SetFont("Game_files/Font/UkrainianPrincess.ttf");
	SetTexture("texture_button", "Game_files/Assets/button/button.png");

	GameObject* shapeBackground = CreateShapeAlone(ShapeType, "BackgroundScene", Maths::Vector2f(0, 0), sf::Color(128, 128, 128, 128), window_size);
	GameObject* endLevelText = CreateText(TextType, "textEndLevel", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 25)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 10)), 40);
	GameObject* nextLevelEndScene = CreateSpriteButtonWithText(ButtonType, "NextLevelButton", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 4 / 6), Maths::Vector2f(button_size_x, button_size_y), [] {}, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "PROCHaIN NIVEaU", sf::Color::Black, 30);
	GameObject* menuButtonEndScene = CreateSpriteButtonWithText(ButtonType, "QuitButton", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 3 / 6), Maths::Vector2f(button_size_x, button_size_y), [this] { LaunchFunction::LaunchScene<MainMenuScene>(); }, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "RETOUR aU MENU", sf::Color::Black, 30);

	endLevelText->GetComponent<TextRenderer>()->SetString("VOUS aVEZ TERMINE LE NIVEaU 3 !");

	nextLevelEndScene->GetComponent<Button>()->is_disabled = true;
	if (nextLevelEndScene->GetComponent<Button>()->is_disabled)
	{
		nextLevelEndScene->GetComponent<SpriteRenderer>()->SetNextSpriteRect(3);
	}
}