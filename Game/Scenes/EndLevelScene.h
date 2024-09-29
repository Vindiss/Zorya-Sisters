#pragma once

#include "Engine.h"
#include "Scene.h"
#include "WindowModule.h"

class EndLevelScene final : public Scene
{
public:
	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();
	float button_size_x = window_size.x / 15 * 3;
	float button_size_y = (((window_size.x / 15) * 3) * 168 / 448);

	EndLevelScene();
};