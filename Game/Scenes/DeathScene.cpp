#include "DeathScene.h"
#include "LaunchFunction.h"

#include "SelectCapacityScene.h"
#include "MainMenuScene.h"

DeathScene::DeathScene() : Scene("DeathScene", [] {}) {
	SetFont("Game_files/Font/UkrainianPrincess.ttf");
	SetTexture("texture_button", "Game_files/Assets/button/button.png");

	GameObject* shapeBackground = CreateShapeAlone(ShapeType, "BackgroundScene", Maths::Vector2f(0, 0), sf::Color(128, 128, 128, 128), window_size);
	GameObject* textPause = CreateText(TextType, "textDead", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 25)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 10)), 40);
	GameObject* checkpointtButton = CreateSpriteButtonWithText(ButtonType, "Checkpoint_button", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 3 / 6), Maths::Vector2f(button_size_x, button_size_y), [this] { LaunchFunction::resumeGame(); }, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "RETOUR aU DERNIER CHECKPOINT", sf::Color::Black, 30);
	GameObject* restartButton = CreateSpriteButtonWithText(ButtonType, "restart_button", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 4 / 6), Maths::Vector2f(button_size_x, button_size_y), [this] { LaunchFunction::LaunchScene<SelectCapacityScene>(); }, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "RECOMMENCER La PaRTIE", sf::Color::Black, 30);
	GameObject* quitButton = CreateSpriteButtonWithText(ButtonType, "quit_button", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 5 / 6), Maths::Vector2f(button_size_x, button_size_y), [this] { LaunchFunction::LaunchScene<MainMenuScene>(); }, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "RETOUR aU MENU", sf::Color::Black, 30);

	SceneModule* sceneModule = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	std::string killedBy = "";

	for (GameObject* const& gameObject : *sceneModule->GetScene("DefaultScene")->GetGameObjects())
	{
		if (gameObject->GetName() == "Player")
		{
			killedBy = gameObject->GetComponent<Player>()->GetDeathBy();
		}
	}

	if (killedBy == "Lampe_Bottom" || killedBy == "Lampe_Top")
	{
		killedBy = "La LaMPE";
	}
	deathMessage += " PaR " + killedBy;
	textPause->GetComponent<TextRenderer>()->SetString(deathMessage);
}