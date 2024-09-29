#pragma once
#include "Capacity.h"
#include <SFML/Audio.hpp>

class Force : public Capacity
{
public:
	Force();
	~Force();

	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;
	void DeplaceObject(const float _delta_time, GameObject* player, GameObject* gameObject, std::vector<GameObject*>* gameObjects);

	void PlaySound();
	void StopSound();

private:
	sf::SoundBuffer* soundBufferForce;
	sf::Sound* soundForce;

	bool soundPlayed = false;
};