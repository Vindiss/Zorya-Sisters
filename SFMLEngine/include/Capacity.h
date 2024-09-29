#pragma once
#include <string>
#include <vector>
#include "GameObject.h"

class GameObject;

class Capacity
{
public:
	Capacity() = default;
	virtual ~Capacity() = default;

	std::string GetName() { return name; }
	std::string GetDescription() { return description; }

	void SetName(std::string new_name);
	void SetDescription(std::string new_description);

	GameObject* GetCapacityOwner() { return capacity_owner; }
	void SetCapacityOwner(GameObject* _owner) { capacity_owner = _owner; }

	virtual void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input){}

private:
	std::string name;
	std::string description;
	GameObject* capacity_owner = nullptr;
};

