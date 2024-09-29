#pragma once
#include "GameObject.h"
#include "Scene.h"


class Interactive : public Component
{
public:
	bool GetOn() { return is_activated; }
	bool GetCanBeActivated() { return can_be_activated; }
	GameObject* GetObject() { return object; }
	std::function<void()> GetCallback() { return callback; }

	void SetOn(bool value) { is_activated = value; }
	void SetCanBeActivated(bool _can_be_activated) { can_be_activated = _can_be_activated; }
	void SetObject(GameObject* _object) { object = _object; }
	void SetCallback(std::function<void()> _callback) { callback = _callback; }

	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;

	void SwitchStateLamp(Scene* scene);
private:
	bool is_activated = false;
	bool can_be_activated = false;
	sf::Clock can_activate_clock;

	GameObject* object;
	GameObject* lampe_Bottom;
	GameObject* lampe_Top;
	std::function<void()> callback;
};