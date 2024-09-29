#pragma once
#pragma once
#include "Scene.h"
#include "Engine.h"
#include <vector>

#include "WindowModule.h"
#include "SceneModule.h"


//class SelectCapacityScene;
//class MainMenuScene;

class DeathScene final : public Scene {
public:

	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();
	float button_size_x = window_size.x / 15 * 3;
	float button_size_y = (((window_size.x / 15) * 3) * 168 / 448);

	std::string deathMessage = "VOUS ETES MORT";


	DeathScene();
};