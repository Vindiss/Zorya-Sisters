#include "TransitionScene.h"
#include "LaunchFunction.h"

#include <fstream>
#include <chrono>
#include "iostream"

TransitionScene::TransitionScene(std::vector<Capacity*>* params) : Scene("TransitionScene", [this] { CallBack(); })
{
	Engine::SetMusicState(false);

	SetTexture("background", "Game_files/Assets/background/capacity_background.png");
	SetBackground("background");
	SetTexture("texture_button", "Game_files/Assets/button/button.png");
	SetFont("Game_files/Font/UkrainianPrincess.ttf");

	GameObject* background = CreateShapeAlone(ShapeType, "shapeBackground", Maths::Vector2f(), sf::Color(128, 128, 128, 128), window_size);

	GameObject * text_tips = CreateText(TextType, "text_tips", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 85)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 10)), 40);


	std::ifstream file(fileName);


	while (std::getline(file, tip)) {
		++nbrTipLines;
	}
	file.close();
	file.open(fileName);

	tipLine = randomNumber(1, nbrTipLines); // Nombre de lignes dans le csv, à automati
	for (int i = 0; i < tipLine; i++)
	{
		std::getline(file, tip);
	}
	text_tips->GetComponent<TextRenderer>()->SetString(tip);

	capacities = params;
}

void TransitionScene::CallBack() {
	LaunchFunction::LaunchGame(capacities);
}

int TransitionScene::randomNumber(int a, int b) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	gen.seed(seed);


	int min = a;
	int max = b;

	std::uniform_int_distribution<int> distribution(min, max);

	int number = distribution(gen);
	return number;
}