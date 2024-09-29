#pragma once
#include "GameObject.h"
#include "Scene.h"
#include <functional>


class Button : public Component
{
public:
	Button() = default;
	~Button() = default;

	void SetObject(Capacity* _object) { object = _object; }
	void SetCallback(std::function<void()> _callback) { callback = _callback; }
	void SetTextObject(GameObject* _text_object) { text_object = _text_object; }

	Capacity* GetObject() { return object; }
	GameObject* GetTextObject() { return text_object; }

	void CheckHover();

	void Execute();
	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input);

	void DollSelectCapacity();
	void SoundMenu();
	void ControlsMenu();
	void SelectCapacity();
	void SliderMove();
	void ChangeEntry();

	bool is_clicked = false;
	bool is_disabled = false;
	GameObject* has_select = nullptr;
	GameObject* is_selected_by = nullptr;
	bool callback_execute = false;

private:

	std::function<void()> callback;
	Capacity* object = nullptr;
	GameObject* text_object = nullptr;
};
