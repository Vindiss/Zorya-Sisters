#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "ARendererComponent.h"

class SpriteRenderer : public ARendererComponent
{
public:
	SpriteRenderer();
	~SpriteRenderer() override;

	float GetHeight() const { return height; }
	float GetWidth() const { return width; }
	sf::Sprite* GetSprite() { return sprite; }
	sf::Texture* GetTextureSprite() { return texture; }
	Maths::Vector2f GetSpriteSize() { return sprite_size; }
	Maths::Vector2f GetSpriteFirstPosition() { return sprite_first_position; }
	int GetActualSpriteInt() { return nb_actual_sprite; }

	void SetWidth(const float _width) { width = _width; }
	void SetHeight(const float _height) { height = _height; }
	void SetOrigin(float x, float y);
	void SetSpriteFirstPosition(Maths::Vector2f _sprite_first_position) { sprite_first_position = _sprite_first_position; }
	void SetActualSpriteInt(int _nb_actual_sprite) { nb_actual_sprite = _nb_actual_sprite; }

	void SetSprite(sf::Texture* new_texture, Maths::Vector2f _size);
	void SetSpriteRect(sf::Texture* new_texture, Maths::Vector2f _size, Maths::Vector2f _sprite_size, Maths::Vector2f _position, Maths::Vector2f _sprite_space);
	void SetNextSpriteRect(int num_sprite_on_sheet);


	void Render(sf::RenderWindow* _window) override;
	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;

private:
	sf::Sprite* sprite = nullptr;
	int nb_actual_sprite = 0;
	sf::Clock animationCooldown;

	sf::Texture* texture = nullptr;
	int width = 100;
	int height = 100;
	Maths::Vector2f sprite_size;
	Maths::Vector2f sprite_space;
	Maths::Vector2f sprite_first_position;
};
