#pragma once

#include "Scene.h"
#include <SFML/Audio.hpp>




//class MainMenuScene;

class SelectCapacityScene final : public Scene
{
public:
	SelectCapacityScene();
	SelectCapacityScene(std::vector<Capacity*>* _capacitys);

private:
	void callbackPlayButton();
	void SetNewCapacitys(std::string _player_capacity);

	sf::Music music;

};