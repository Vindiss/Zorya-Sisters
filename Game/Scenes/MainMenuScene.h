#pragma once

#include "Scene.h"
#include <SFML/Audio.hpp>

class MainMenuScene final : public Scene
{
public:
	MainMenuScene();
	sf::Music music;
};