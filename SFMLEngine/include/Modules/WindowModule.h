#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>
#include <SFML/Window/Event.hpp>

#include "Module.h"
#include "GameObject.h"

class WindowModule final : public Module
{
public:
	
	std::unordered_map<sf::Keyboard::Scancode, bool>* GetPressed() { return &pressed; }
	void ResetButton(GameObject* button, std::vector<std::string> _name);
	sf::Vector2i GetMousePosition();
	void Init() override;
	void Start() override;
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Release() override;

	sf::RenderWindow* GetWindow() const { return window; }
	sf::Vector2f GetWindowSize() const { return sf::Vector2f(window->getSize().x, window->getSize().y); };
	std::vector<sf::Vector2i> mousePos;

private:
	sf::RenderWindow* window = nullptr;
	std::unordered_map<sf::Keyboard::Scancode, bool> pressed;

};
