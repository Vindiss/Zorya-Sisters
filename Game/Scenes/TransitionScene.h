#pragma once
#include "Scene.h"
#include "Engine.h"
#include "WindowModule.h"

#include <random>

class TransitionScene : public Scene {
private:
	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();
	std::mt19937 gen; // Mersenne Twister 19937 comme générateur pseudo-aléatoire
	std::vector<Capacity*>* capacities = new std::vector<Capacity*>;


	std::string fileName = "Game_files/transition_tips.csv";
	int tipLine;
	int nbrTipLines = 0;
	std::string tip;

	void CallBack();
	int randomNumber(int a, int b);
	

public:
	TransitionScene(std::vector<Capacity*>* params);
};