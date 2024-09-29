#pragma once
#include "Scene.h"
#include "Engine.h"
#include "WindowModule.h"
#include "TimeModule.h"

class IntroScene : public Scene {
private:
	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();

	sf::Clock clock;

	GameObject* logo = nullptr;
	GameObject* text_intro = nullptr;
	GameObject* shape_background = nullptr;

	void introCallback();
public:
	IntroScene();
};