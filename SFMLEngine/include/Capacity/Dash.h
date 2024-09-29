// Dash.h
#pragma once
#include "Capacity.h"
#include <SFML/System/Clock.hpp>
#include "GameObject.h"
#include <SFML/Audio.hpp>

#include "Engine.h"
#include "Modules/WindowModule.h"

class Dash : public Capacity {
public:
    Dash();
    ~Dash();

    void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;

    void PlaySound();
    void StopSound();

private:
    sf::Clock StartClockCooldownDash;
    int CooldownDash = 1000;
    sf::Clock StartClockDurationDash;
    int speed_dash = 0;
    int timeDash = 50;

    sf::SoundBuffer* soundBufferDash;
    sf::Sound* soundDash;

    bool soundPlayed = false;
    bool is_dashing = false;

    void IsDashing(const float _delta_time, std::vector<GameObject*>* gameObjects);

    sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();
};
