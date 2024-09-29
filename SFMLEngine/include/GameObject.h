#pragma once
#include <string>
#include <vector>
#include "functional"

#include "Component.h"
#include "Maths/Vector2.h"
#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Components/ObjectType.h"
#include "Capacity.h"

class Component;
class Capacity;

class GameObject
{
public:
	GameObject() = default;
	~GameObject();

	ObjectType GetType() const { return type; }
	std::string GetName() const { return name; }
	Maths::Vector2<float> GetPosition() const { return position; }
	float GetRotation() const { return rotation; }
	Maths::Vector2<float> GetScale() const { return scale; }
	bool GetSwitchOn() { return switchOn; }
	bool GetCallbackProc() { return callbackProc; }
	std::function<void()> GetCallback() { return callback; }
	bool GetIsAnimated() { return is_animated; }
	int GetNbAnimationSprite() { return nb_animation_sprite; }
	bool GetAnimatedLoop() { return animated_loop; }
	bool GetReverseAnimation() { return reverse_animation; }

	void SetType(const ObjectType& _type) { type = _type; }
	void SetName(const std::string& _name) { name = _name; }
	void SetPosition(const Maths::Vector2<float>& _position) { position = _position; }
	void SetRotation(const float _rotation) { rotation = _rotation; }
	void SetScale(const Maths::Vector2<float>& _scale) { scale = _scale; }
	void SetSwitchOn(bool value) {switchOn = value;}
	void SetCallback(std::function<void()> _callback) { callback = _callback; }
	void SetCallbackProc(bool new_bool) { callbackProc = new_bool; }
	void SetIsAnimated(bool _is_animated) { is_animated = _is_animated; }
	void SetNbAnimationSprite(int _nb_animation_sprite) { nb_animation_sprite = _nb_animation_sprite; }
	void SetAnimatedLoop(bool _animated_loop) { animated_loop = _animated_loop; }
	void SetReverseAnimation(bool _reverse_animation) { reverse_animation = _reverse_animation; }

	template<typename T>
	T* CreateComponent();

	template<typename T>
	T* GetComponent();

	void AddComponent(Component* _component);
	void RemoveComponent(Component* _component);

	void Update(float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) const;
	void Render(sf::RenderWindow* _window) const;
	sf::FloatRect getBounds(sf::RectangleShape object) const;

private:
	ObjectType type = ObjectType::GameObjectType;
	std::string name = "GameObject";

	Maths::Vector2<float> position = Maths::Vector2f::Zero;
	float rotation = 0.0f;
	Maths::Vector2<float> scale = Maths::Vector2f::One;

	std::vector<Component*> components;

	std::function<void()> callback;
	bool callbackProc = false;

	bool switchOn = false;

	bool is_animated = false;
	int nb_animation_sprite = 0;
	bool animated_loop = false;
	bool reverse_animation = false;

};

template<typename T>
T* GameObject::CreateComponent()
{
	T* component = new T();
	component->SetOwner(this);
	components.push_back(component);
	return component;
}

template<typename T>
T* GameObject::GetComponent()
{
	for (Component* component : components)
	{
		T* result = dynamic_cast<T*>(component);
		if (result != nullptr)
			return result;
	}

	return nullptr;
}
