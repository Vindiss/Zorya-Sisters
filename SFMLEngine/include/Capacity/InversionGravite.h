#pragma once
#include "Capacity.h"
#include <Scene.h>
#include <SFML/Audio.hpp>

class InversionGravite : public Capacity
{
public:
	InversionGravite();
	~InversionGravite();

	void GraviteInversion(GameObject* player, const float _delta_time, Scene* scene);
	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;
	void SetCount(int _new_count) { count = _new_count; }

	void PlaySound();
	void StopSound();

private:
	bool inversion = false;
	int count = 0;
	sf::Clock inversionClock;

	bool soundPlayed = false;
	sf::SoundBuffer* soundBufferInversionGravite;
	sf::Sound* soundInversionGravite;
};
