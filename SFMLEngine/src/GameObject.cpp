#include "GameObject.h"

GameObject::~GameObject()
{
	for (Component*& component : components)
		delete component;

	components.clear();
}

void GameObject::AddComponent(Component* _component)
{
	_component->SetOwner(this);
	components.push_back(_component);
}

void GameObject::RemoveComponent(Component* _component)
{
	components.erase(std::remove(components.begin(), components.end(), _component), components.end());
}


void GameObject::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) const
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update(_delta_time, pressed_input);
	}
}

void GameObject::Render(sf::RenderWindow* _window) const
{
	for (Component* const& component : components)
	{
		component->Render(_window);
	}
}

sf::FloatRect GameObject::getBounds(sf::RectangleShape object) const
{
	sf::Vector2f position = object.getPosition();
	sf::Vector2f size = object.getSize();

	return sf::FloatRect(position.x, position.y, size.x, size.y);
}
