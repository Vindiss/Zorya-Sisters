#pragma once
#include <SFML/System/Clock.hpp>

#include "Module.h"

class TimeModule final : public Module
{
public:
	TimeModule() = default;
	~TimeModule() = default;

	void Init() override;
	void Update() override;

	float GetDeltaTime() const;
	float GetTimeSinceBeginning() const;

	sf::Clock GetDeltaClock() const;
	sf::Clock GetClockSinceBeginning() const;

private:
	sf::Clock deltaClock;
	sf::Clock clock;

	float deltaTime = 0.0f;
};
