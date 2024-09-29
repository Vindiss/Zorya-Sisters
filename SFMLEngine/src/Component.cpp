#include "Component.h"

Component::Component(GameObject* _owner)
{
	this->owner = _owner;
}

void Component::Update(float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {}

void Component::Render(sf::RenderWindow* _window) {}
