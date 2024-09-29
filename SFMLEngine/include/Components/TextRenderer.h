#pragma once

#include <SFML/Graphics.hpp>

#include "ARendererComponent.h"

class TextRenderer : public ARendererComponent
{
public:
	TextRenderer();
	~TextRenderer() override;

	void SetColor(const sf::Color& _color) { color = _color; }
	void SetString(std::string _string) { string = _string; }
	void SetCaracterSize(int _caractere_size) { caractere_size = _caractere_size; }
	void SetTextName(std::string _text_name) { text_name = _text_name; }
	void SetSpecialPosition(Maths::Vector2f _special_position) { special_position = _special_position; }

	sf::Text* GetText() { return text; }
	std::string GetString() { return string; }

	void Render(sf::RenderWindow* _window) override;

private:
	std::string text_name = "";
	Maths::Vector2f special_position = Maths::Vector2f(0, 0);

	sf::Color color = sf::Color::White;
	int caractere_size = 20;

	sf::Text* text = nullptr;
	std::string string = "";
};