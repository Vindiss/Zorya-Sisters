#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Modules/WindowModule.h"


class DollOff : public Component
{
public:
	Maths::Vector2f getSize() { return size; }
	void setSize(Maths::Vector2f _size) { size = _size; }

	sf::Color getColor() { return color; }
	void setColor(sf::Color _color) { color = _color; }

	void SetHp(int new_hp) { hp = new_hp; }
	void SetSpeed(int new_speed) { speed = new_speed; }


	Maths::Vector2f getPosition() { return size; }
	void setPosition(Maths::Vector2f _position) { position = _position; }

	int Doll() {
		sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();
		speed = window_size.x / 15;
	}

private:

	Maths::Vector2f size = Maths::Vector2f(200, 200);
	sf::Color color = sf::Color::Transparent;
	Maths::Vector2f position = Maths::Vector2f(0, 0);
	int hp = 0;
	int speed = 0;
	int gravity = 100;
};

