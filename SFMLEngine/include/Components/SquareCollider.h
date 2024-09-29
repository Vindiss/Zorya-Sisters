#pragma once
#include "SFML/Graphics.hpp"
#include "Component.h"

class SquareCollider : public Component
{
public:
	SquareCollider() = default;
	~SquareCollider() override = default;

	float width = 1.0f;
	float height = 1.0f;

	float GetWidth() const { return width; }
	float GetHeight() const { return height; }
	std::unordered_map<std::string, bool> GetCanMoving() { return CanMoving; }
	Maths::Vector2f GetSpecialPosition() { return special_position; }

	void SetWidth(const float _width) { width = _width; }
	void SetHeight(const float _height) { height = _height; }
	void SetCanMoving(std::string direction, bool move) { CanMoving[direction] = move; }
	void SetSpecialPosition(Maths::Vector2f _special_position) { special_position = _special_position; }

	static bool IsColliding(const SquareCollider& _collider_a, const SquareCollider& _collider_b, const float _delta_time);
	static bool CheckCollisionBottom(const SquareCollider& _collider_a, const SquareCollider& _collider_b);
	static bool CheckCollisionRight(const SquareCollider& _collider_a, const SquareCollider& _collider_b);
	static bool CheckCollisionLeft(const SquareCollider& _collider_a, const SquareCollider& _collider_b);



private:
	std::unordered_map<std::string, bool> CanMoving = { {"up", true}, {"down", true}, {"left", true}, {"right", true} };
	Maths::Vector2f special_position = Maths::Vector2f(0, 0);
};
