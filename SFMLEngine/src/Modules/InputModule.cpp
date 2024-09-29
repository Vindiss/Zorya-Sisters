#include "Modules/InputModule.h"

#include "ModuleManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Modules/WindowModule.h"
#include <SFML/Window.hpp>


Input::Input(std::string _name, sf::Keyboard::Scancode _key) {
	name = _name;
	key = _key;
}

Input::Input(std::string _name, sf::Mouse::Button _button) {
	name = _name;
	button = _button;
}
 

int Input::GetEntry() {
	if (name != "MB1" && name != "MB2" && name != "MB3" && name != "MB4" && name != "MB5") {
		return key;
	}
	else {
		return button;
	}
}

bool Input::EntryIsPressesd() {
	if (name != "MB1" && name != "MB2" && name != "MB3" && name != "MB4" && name != "MB5") {
		return sf::Keyboard::isKeyPressed(key);
	}
	else {
		return sf::Mouse::isButtonPressed(button);
	}
}

void InputModule::Start()
{
	Module::Start();

	window = moduleManager->GetModule<WindowModule>()->GetWindow();

	std::ifstream file("Game_files/settings.csv");

	if (!file.is_open()) {
		std::cerr << "Error opening file." << std::endl;
	}

	std::string line;

	file.clear();
	file.seekg(0, std::ios::beg);

	// Ignorer les deux premières lignes
	for (int i = 0; i < 2; ++i) {
		std::string ligne;
		std::getline(file, ligne);
	}

	while (std::getline(file, line)) {
		std::vector<std::string> row;
		std::stringstream lineStream(line);
		std::string cell;

		while (std::getline(lineStream, cell, ',')) {
			row.push_back(cell);
		}
		if (row[1] != "MB1" && row[1] != "MB2" && row[1] != "MB3" && row[1] != "MB4" && row[1] != "MB5") {
			sf::Keyboard::Scancode key = static_cast<sf::Keyboard::Scancode>(stoi(row[2]));
			std::cout << key << std::endl;
			Input* control = new Input(row[1], key);
			controls[row[0]] = control;
		}
		else {
			sf::Mouse::Button button = static_cast<sf::Mouse::Button>(stoi(row[2]));
			Input* control = new Input(row[1], button);
			controls[row[0]] = control;
		}
	}

	file.close();
}

void InputModule::Update()
{
	Module::Update();
}
