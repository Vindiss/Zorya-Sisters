#include "Components/SquareCollider.h"
#include "Engine.h"
#include "Maths/Vector2.h"
#include <iostream>
#include <Components/Player.h>
#include <Components/Interactive.h>

bool SquareCollider::IsColliding(const SquareCollider& _collider_a, const SquareCollider& _collider_b, const float _delta_time)
{
	sf::RectangleShape rPlayer;
	sf::RectangleShape rObject;

	Maths::Vector2f _collider_a_position;
	if (_collider_a.special_position.x != 0) {
		_collider_a_position.SetX(_collider_a.GetOwner()->GetPosition().GetX() + _collider_a.special_position.x);
	}
	else {
		_collider_a_position.SetX(_collider_a.GetOwner()->GetPosition().GetX());
	}
	if (_collider_a.special_position.y != 0) {
		_collider_a_position.SetY(_collider_a.GetOwner()->GetPosition().GetY() +  _collider_a.special_position.y);
	}
	else {
		_collider_a_position.SetY(_collider_a.GetOwner()->GetPosition().GetY());
	}
	Maths::Vector2f _collider_b_position;
	if (_collider_b.special_position.x != 0) {
		_collider_b_position.SetX(_collider_b.GetOwner()->GetPosition().GetX() + _collider_b.special_position.x);
	}
	else {
		_collider_b_position.SetX(_collider_b.GetOwner()->GetPosition().GetX());
	}
	if (_collider_b.special_position.y != 0) {
		_collider_b_position.SetY(_collider_b.GetOwner()->GetPosition().GetY() + _collider_b.special_position.y);
	}
	else {
		_collider_b_position.SetY(_collider_b.GetOwner()->GetPosition().GetY());
	}

	rPlayer.setPosition(sf::Vector2f(_collider_a_position.x, _collider_a_position.y));
	rPlayer.setSize(sf::Vector2f(_collider_a.GetWidth(), _collider_a.GetHeight()));
	rObject.setPosition(sf::Vector2f(_collider_b_position.x, _collider_b_position.y));
	rObject.setSize(sf::Vector2f(_collider_b.GetWidth(), _collider_b.GetHeight()));

	sf::FloatRect playerBounds = _collider_a.GetOwner()->getBounds(rPlayer);
	sf::FloatRect objectBounds = _collider_b.GetOwner()->getBounds(rObject);

	bool collison_for_jumping_obj = false;
	bool player_jump = false;
	if (_collider_a.GetOwner()->GetComponent<Player>() != nullptr)
	{
		player_jump = _collider_a.GetOwner()->GetComponent<Player>()->GetIsJumping();
	}
	else
	{
		player_jump = false;
	}

	if (_collider_b.GetOwner()->GetType() == InteractiveType && _collider_a.GetOwner()->GetType() == PlayerType) {
		if (playerBounds.intersects(objectBounds)) {
			_collider_b.GetOwner()->GetComponent<Interactive>()->SetCanBeActivated(true);
		}
		else {
			_collider_b.GetOwner()->GetComponent<Interactive>()->SetCanBeActivated(false);
		}
	}
	else {
		if(playerBounds.intersects(objectBounds))
		{
			int collisionWidth = std::min(playerBounds.left + playerBounds.width, objectBounds.left + objectBounds.width) - std::max(playerBounds.left, objectBounds.left);
			int collisionHeight = std::min(playerBounds.top + playerBounds.height, objectBounds.top + objectBounds.height) - std::max(playerBounds.top, objectBounds.top);
			// top collision with loss of life
			if (playerBounds.top > objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height && ((playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left) || (playerBounds.left >= objectBounds.left && playerBounds.left <= objectBounds.left + objectBounds.width)) && (collisionWidth > collisionHeight && collisionWidth > 5) && _collider_b.GetOwner()->GetType() == DeathType)
			{
				if (_collider_a.GetOwner()->GetComponent<Player>()->GetCapacity()->GetName() == "Invincibilite")
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("up", false);
				}
				else
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("up", false);
					_collider_a.GetOwner()->GetComponent<Player>()->SetHp(_collider_a.GetOwner()->GetComponent<Player>()->GetHp() - (100));
					_collider_a.GetOwner()->GetComponent<Player>()->setDeathBy(_collider_b.GetOwner()->GetName());
				}
			}
			// top collision
			else if (playerBounds.top > objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height && ((playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left) || (playerBounds.left >= objectBounds.left && playerBounds.left <= objectBounds.left + objectBounds.width)) && (collisionWidth > collisionHeight && collisionWidth > 5))
			{
				if (player_jump && _collider_b.GetOwner()->GetName() == "Lit_appuis_tete" || _collider_b.GetOwner()->GetName() == "Bureau") {
					collison_for_jumping_obj = true;
				}
				else {
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("up", false);
				}
			}
			// bottom collision with loss of life
			if (playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top && ((playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left) || (playerBounds.left >= objectBounds.left && playerBounds.left <= objectBounds.left + objectBounds.width)) && (collisionWidth > collisionHeight && collisionWidth > 5) && _collider_b.GetOwner()->GetType() == DeathType)
			{
				if (_collider_a.GetOwner()->GetComponent<Player>()->GetCapacity()->GetName() == "Invincibilite")
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", false);
				}
				else
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", false);
					_collider_a.GetOwner()->GetComponent<Player>()->SetHp(_collider_a.GetOwner()->GetComponent<Player>()->GetHp() - (100));
					_collider_a.GetOwner()->GetComponent<Player>()->setDeathBy(_collider_b.GetOwner()->GetName());
				}
			}
			// bottom collision
			else if (playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top && ((playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left) || (playerBounds.left >= objectBounds.left && playerBounds.left <= objectBounds.left + objectBounds.width)) && (collisionWidth > collisionHeight && collisionWidth > 5))
			{
				if ((player_jump && _collider_b.GetOwner()->GetName() == "Lit_appuis_tete" || _collider_b.GetOwner()->GetName() == "Bureau")) {
					if (playerBounds.top + playerBounds.height - 0.25 <= objectBounds.top) {
						_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", false);
					}
				}
				else if (_collider_b.GetOwner()->GetName() == "Trou_de_souris")
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", false);

					if (!_collider_b.GetOwner()->GetCallbackProc())
					{
						_collider_b.GetOwner()->GetCallback()();
						_collider_b.GetOwner()->SetCallbackProc(true);
					}
				}
				else {
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", false);
					_collider_a.GetOwner()->SetPosition(Maths::Vector2f(_collider_a.GetOwner()->GetPosition().x, _collider_a.GetOwner()->GetPosition().y - collisionHeight));
				}
			}
			//left collision with loss of life 
			if (playerBounds.left >= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5) && _collider_b.GetOwner()->GetType() == DeathType)
			{
				if (_collider_a.GetOwner()->GetComponent<Player>()->GetCapacity()->GetName() == "Invincibilite")
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("left", false);
				}
				else
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("left", false);
					_collider_a.GetOwner()->GetComponent<Player>()->SetHp(_collider_a.GetOwner()->GetComponent<Player>()->GetHp() - (100));
					_collider_a.GetOwner()->GetComponent<Player>()->setDeathBy(_collider_b.GetOwner()->GetName());
				}
			}
			//left collision 
			else if (playerBounds.left >= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5))
			{
				_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("left", false);

			}
			//right collision with loss of life
			if (playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width <= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5) && _collider_b.GetOwner()->GetType() == DeathType)
			{
				if (_collider_a.GetOwner()->GetComponent<Player>()->GetCapacity()->GetName() == "Invincibilite")
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("right", false);
				}
				else
				{
					_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("right", false);
					_collider_a.GetOwner()->GetComponent<Player>()->SetHp(_collider_a.GetOwner()->GetComponent<Player>()->GetHp() - (100));
					_collider_a.GetOwner()->GetComponent<Player>()->setDeathBy(_collider_b.GetOwner()->GetName());
				}
			}
			//right collision
			else if (playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width <= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5))
			{
				_collider_a.GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("right", false);
			}
			return true;
		}
		return false;
	}
}


bool SquareCollider::CheckCollisionBottom(const SquareCollider& _collider_a, const SquareCollider& _collider_b) {
	sf::RectangleShape rPlayer;
	sf::RectangleShape rObject;

	Maths::Vector2f _collider_a_position;
	if (_collider_a.special_position.x != 0) {
		_collider_a_position.SetX(_collider_a.special_position.x);
	}
	else {
		_collider_a_position.SetX(_collider_a.GetOwner()->GetPosition().GetX());
	}
	if (_collider_a.special_position.y != 0) {
		_collider_a_position.SetY(_collider_a.special_position.y);
	}
	else {
		_collider_a_position.SetY(_collider_a.GetOwner()->GetPosition().GetY());
	}
	Maths::Vector2f _collider_b_position;
	if (_collider_b.special_position.x != 0) {
		_collider_b_position.SetX(_collider_b.special_position.x);
	}
	else {
		_collider_b_position.SetX(_collider_b.GetOwner()->GetPosition().GetX());
	}
	if (_collider_b.special_position.y != 0) {
		_collider_b_position.SetY(_collider_b.special_position.y);
	}
	else {
		_collider_b_position.SetY(_collider_b.GetOwner()->GetPosition().GetY());
	}

	rPlayer.setPosition(sf::Vector2f(_collider_a_position.x, _collider_a_position.y));
	rPlayer.setSize(sf::Vector2f(_collider_a.GetWidth(), _collider_a.GetHeight()));
	rObject.setPosition(sf::Vector2f(_collider_b_position.x, _collider_b_position.y));
	rObject.setSize(sf::Vector2f(_collider_b.GetWidth(), _collider_b.GetHeight()));

	sf::FloatRect playerBounds = _collider_a.GetOwner()->getBounds(rPlayer);
	sf::FloatRect objectBounds = _collider_b.GetOwner()->getBounds(rObject);

	if (playerBounds.intersects(objectBounds))
	{
		int collisionWidth = std::min(playerBounds.left + playerBounds.width, objectBounds.left + objectBounds.width) - std::max(playerBounds.left, objectBounds.left);
		int collisionHeight = std::min(playerBounds.top + playerBounds.height, objectBounds.top + objectBounds.height) - std::max(playerBounds.top, objectBounds.top);
		if (playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top && ((playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left) || (playerBounds.left >= objectBounds.left && playerBounds.left <= objectBounds.left + objectBounds.width)) && (collisionWidth > collisionHeight && collisionWidth > 5))
		{
			return true;
		}
	}
	return false;
}


bool SquareCollider::CheckCollisionRight(const SquareCollider& _collider_a, const SquareCollider& _collider_b) {
	sf::RectangleShape rPlayer;
	sf::RectangleShape rObject;

	Maths::Vector2f _collider_a_position;
	if (_collider_a.special_position.x != 0) {
		_collider_a_position.SetX(_collider_a.special_position.x);
	}
	else {
		_collider_a_position.SetX(_collider_a.GetOwner()->GetPosition().GetX());
	}
	if (_collider_a.special_position.y != 0) {
		_collider_a_position.SetY(_collider_a.special_position.y);
	}
	else {
		_collider_a_position.SetY(_collider_a.GetOwner()->GetPosition().GetY());
	}
	Maths::Vector2f _collider_b_position;
	if (_collider_b.special_position.x != 0) {
		_collider_b_position.SetX(_collider_b.special_position.x);
	}
	else {
		_collider_b_position.SetX(_collider_b.GetOwner()->GetPosition().GetX());
	}
	if (_collider_b.special_position.y != 0) {
		_collider_b_position.SetY(_collider_b.special_position.y);
	}
	else {
		_collider_b_position.SetY(_collider_b.GetOwner()->GetPosition().GetY());
	}

	rPlayer.setPosition(sf::Vector2f(_collider_a_position.x, _collider_a_position.y));
	rPlayer.setSize(sf::Vector2f(_collider_a.GetWidth(), _collider_a.GetHeight()));
	rObject.setPosition(sf::Vector2f(_collider_b_position.x, _collider_b_position.y));
	rObject.setSize(sf::Vector2f(_collider_b.GetWidth(), _collider_b.GetHeight()));

	sf::FloatRect playerBounds = _collider_a.GetOwner()->getBounds(rPlayer);
	sf::FloatRect objectBounds = _collider_b.GetOwner()->getBounds(rObject);

	if (playerBounds.intersects(objectBounds))
	{
		int collisionWidth = std::min(playerBounds.left + playerBounds.width, objectBounds.left + objectBounds.width) - std::max(playerBounds.left, objectBounds.left);
		int collisionHeight = std::min(playerBounds.top + playerBounds.height, objectBounds.top + objectBounds.height) - std::max(playerBounds.top, objectBounds.top);
		if (playerBounds.left <= objectBounds.left && playerBounds.left + playerBounds.width <= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5))
		{
			return true;
		}
	}
	return false;
}

bool SquareCollider::CheckCollisionLeft(const SquareCollider& _collider_a, const SquareCollider& _collider_b) {
	sf::RectangleShape rPlayer;
	sf::RectangleShape rObject;

	Maths::Vector2f _collider_a_position;
	if (_collider_a.special_position.x != 0) {
		_collider_a_position.SetX(_collider_a.special_position.x);
	}
	else {
		_collider_a_position.SetX(_collider_a.GetOwner()->GetPosition().GetX());
	}
	if (_collider_a.special_position.y != 0) {
		_collider_a_position.SetY(_collider_a.special_position.y);
	}
	else {
		_collider_a_position.SetY(_collider_a.GetOwner()->GetPosition().GetY());
	}
	Maths::Vector2f _collider_b_position;
	if (_collider_b.special_position.x != 0) {
		_collider_b_position.SetX(_collider_b.special_position.x);
	}
	else {
		_collider_b_position.SetX(_collider_b.GetOwner()->GetPosition().GetX());
	}
	if (_collider_b.special_position.y != 0) {
		_collider_b_position.SetY(_collider_b.special_position.y);
	}
	else {
		_collider_b_position.SetY(_collider_b.GetOwner()->GetPosition().GetY());
	}

	rPlayer.setPosition(sf::Vector2f(_collider_a_position.x, _collider_a_position.y));
	rPlayer.setSize(sf::Vector2f(_collider_a.GetWidth(), _collider_a.GetHeight()));
	rObject.setPosition(sf::Vector2f(_collider_b_position.x, _collider_b_position.y));
	rObject.setSize(sf::Vector2f(_collider_b.GetWidth(), _collider_b.GetHeight()));

	sf::FloatRect playerBounds = _collider_a.GetOwner()->getBounds(rPlayer);
	sf::FloatRect objectBounds = _collider_b.GetOwner()->getBounds(rObject);

	if (playerBounds.intersects(objectBounds))
	{
		int collisionWidth = std::min(playerBounds.left + playerBounds.width, objectBounds.left + objectBounds.width) - std::max(playerBounds.left, objectBounds.left);
		int collisionHeight = std::min(playerBounds.top + playerBounds.height, objectBounds.top + objectBounds.height) - std::max(playerBounds.top, objectBounds.top);
		if (playerBounds.left >= objectBounds.left && playerBounds.left + playerBounds.width >= objectBounds.left + objectBounds.width && ((playerBounds.top <= objectBounds.top && playerBounds.top + playerBounds.height >= objectBounds.top) || (playerBounds.top >= objectBounds.top && playerBounds.top <= objectBounds.top + objectBounds.height)) && (collisionWidth < collisionHeight && collisionHeight > 5))
		{
			return true;
		}
	}
	return false;
}