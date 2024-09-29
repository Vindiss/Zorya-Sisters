#include "IntroScene.h"
#include "LaunchFunction.h"
#include "MainMenuScene.h"

IntroScene::IntroScene() : Scene("IntroScene", [this] {introCallback(); }) {
	SetTexture("background", "Game_files/Assets/background/capacity_background.png");
	SetBackground("background");
	SetFont("Game_files/Font/UkrainianPrincess.ttf");
	SetTexture("texture_logo_darwins", "Game_files/Assets/logos/darwin.png");
	SetTexture("texture_logo_jeu", "Game_files/Assets/logos/logo_jeu.png");
	
	shape_background = CreateShapeAlone(ShapeType, "shapeFondu", Maths::Vector2f(), sf::Color(128, 128, 128, 128), window_size);

	text_intro = CreateText(TextType, "text_intro", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 25)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 10)), 40);
	logo = CreateOnlySprite(this, GameObjectType, "logoDarwins", Maths::Vector2f(window_size.x / 2 - window_size.y / 3 / 2, window_size.y * 2 / 5), Maths::Vector2f(window_size.y / 3, window_size.y / 3), "texture_logo_darwins", Maths::Vector2f(500, 500), Maths::Vector2f());
	text_intro->GetComponent<TextRenderer>()->SetString("PRESENTE PaR");

}

void IntroScene::introCallback() {
	if (clock.getElapsedTime().asSeconds() > 6.0) {
		LaunchFunction::LaunchScene<MainMenuScene>();
	}
	else if (clock.getElapsedTime().asSeconds() > 3.0) {
		logo->GetComponent<SpriteRenderer>()->SetSprite(GetTextureByName("texture_logo_jeu"), Maths::Vector2f(window_size.y / 3, window_size.y / 3));
		text_intro->GetComponent<TextRenderer>()->SetString("ZORYa SISTERS");
	}
}