#include "Components/SpriteRenderer.h"
#include "SFML/Graphics.hpp"

SpriteRenderer::SpriteRenderer()
{
	sprite = new sf::Sprite();
	texture = new sf::Texture();
}

SpriteRenderer::~SpriteRenderer()
{
	delete sprite;
	sprite = nullptr;
	texture = nullptr;
}

void SpriteRenderer::SetSprite(sf::Texture* new_texture, Maths::Vector2f _size)
{
	texture = new_texture;
	width = _size.x;
	height = _size.y;
	sprite->setTexture(*texture);
	sprite->setScale(_size.x / texture->getSize().x, _size.y / texture->getSize().y);
}

void SpriteRenderer::SetSpriteRect(sf::Texture* new_texture, Maths::Vector2f _size, Maths::Vector2f _sprite_size, Maths::Vector2f _position, Maths::Vector2f _sprite_space)
{
	texture = new_texture;
	width = _size.x;
	height = _size.y;
	sprite_first_position = _position;
	sprite_size = _sprite_size;
	sprite_space = _sprite_space;
	sprite->setTexture(*texture);
	sprite->setTextureRect(sf::IntRect(_position.x, _position.y, _sprite_size.x, _sprite_size.y));
	sprite->setScale(_size.x / _sprite_size.x, _size.y / _sprite_size.y);
}

void SpriteRenderer::SetNextSpriteRect(int num_sprite_on_sheet) {
	nb_actual_sprite = num_sprite_on_sheet;
	float new_pos_x = 0;
	float new_pos_y = 0;
	if (sprite_space.x != 0) {
		new_pos_x = sprite_first_position.x + (num_sprite_on_sheet * (sprite_size.x + sprite_space.x));
	}
	else {
		new_pos_x = sprite_first_position.x + (num_sprite_on_sheet * sprite_space.x);
	}
	if (sprite_space.y != 0) {
		new_pos_y = sprite_first_position.y + (num_sprite_on_sheet * (sprite_size.y + sprite_space.y));
	}
	else {
		new_pos_y = sprite_first_position.y + (num_sprite_on_sheet * sprite_space.y);
	}
	sprite->setTextureRect(sf::IntRect(new_pos_x, new_pos_y, sprite_size.x, sprite_size.y));
	sprite->setScale(width / sprite_size.x, height / sprite_size.y);
}

void SpriteRenderer::Render(sf::RenderWindow* _window)
{
	Component::Render(_window);

	sprite->setPosition(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY());
	_window->draw(*sprite);
}

void SpriteRenderer::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	if (GetOwner()->GetIsAnimated()) {
		if (animationCooldown.getElapsedTime().asSeconds() >= 0.1) {
			if (GetOwner()->GetReverseAnimation()) {
				if (nb_actual_sprite > 0) {
					SetNextSpriteRect(nb_actual_sprite - 1);
				}
				else if (GetOwner()->GetAnimatedLoop()) {
					SetNextSpriteRect(GetOwner()->GetNbAnimationSprite() - 1);
				}
			}
			else {
				if (nb_actual_sprite < GetOwner()->GetNbAnimationSprite() - 1) {
					SetNextSpriteRect(nb_actual_sprite + 1);
				}
				else if (GetOwner()->GetAnimatedLoop()) {
					SetNextSpriteRect(0);
				}
			}
			animationCooldown.restart();
		}
	}
}

void SpriteRenderer::SetOrigin(float x, float y)
{
	sprite->setOrigin(x, y);
}