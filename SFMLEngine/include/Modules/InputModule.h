#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <map>

#include "Module.h"

class Input
{
public:
	Input() = default;
	Input(std::string _name, sf::Keyboard::Scancode _key);
	Input(std::string _name, sf::Mouse::Button _button);


	void SetName(std::string _name) { name = _name; }
	void SetKey(sf::Keyboard::Scancode _key) { key = _key; }
	void SetButton(sf::Mouse::Button _button) { button = _button; }

	std::string GetName() { return name; }
	sf::Keyboard::Scancode GetKey() { return key; }
	sf::Mouse::Button GetButton() { return button; }

	int GetEntry();
	bool EntryIsPressesd();


private:
	std::string name = "";
	sf::Keyboard::Scancode key;
	sf::Mouse::Button button;
};


class InputModule final : public Module
{
public:
	void Start() override;
	void Update() override;

	std::map<std::string, Input*>* GetControls() { return &controls; }

private:
	std::map<std::string, Input*> controls;

	sf::RenderWindow* window = nullptr;
};
