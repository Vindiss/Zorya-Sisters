#include "Capacity/Dash.h"
#include "Components/Player.h"
#include <iostream>

//Sound
Dash::Dash() {
    soundBufferDash = new sf::SoundBuffer;
    if (!soundBufferDash->loadFromFile("Game_files/Assets/Sons/dash.wav")) {
        std::cout << "erreur de chargement du fichier" << std::endl;
    }
    soundDash = new sf::Sound;
}

Dash::~Dash() {
    delete soundBufferDash;
    delete soundDash;
}

void Dash::PlaySound() {
    soundDash->setBuffer(*soundBufferDash);
    soundDash->play();
}

void Dash::StopSound() {
    soundDash->stop();
}


void Dash::IsDashing(const float _delta_time, std::vector<GameObject*>* gameObjects) {
    Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
    GameObject* player = nullptr;

    for (GameObject* const& gameObject : *gameObjects) {
        if (gameObject->GetType() == PlayerType) {
            player = gameObject;
            break;
        }
    }

    if (player != nullptr) {
        if ((speed_dash > 0 && !player->GetComponent<SquareCollider>()->GetCanMoving()["right"]) || (speed_dash < 0 && !player->GetComponent<SquareCollider>()->GetCanMoving()["left"]) || (StartClockDurationDash.getElapsedTime().asMilliseconds() >= timeDash && is_dashing))
        {
            is_dashing = false;
            StartClockCooldownDash.restart();
        }

        if (StartClockCooldownDash.getElapsedTime().asMilliseconds() < CooldownDash)
        {
            SpriteRenderer* spriteRenderer_currentPower = nullptr;
            for (GameObject* const& gameObject : *scene->GetGameObjects())
            {
                if (gameObject->GetName() == "pouvoir en cours")
                {
                    spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
                }
            }
            spriteRenderer_currentPower->SetNextSpriteRect(1);
        }

        // Si le dash est possible et n'est pas en cours
        if (StartClockCooldownDash.getElapsedTime().asMilliseconds() >= CooldownDash && !is_dashing) {


            SpriteRenderer* spriteRenderer_currentPower = nullptr;
            for (GameObject* const& gameObject : *scene->GetGameObjects())
            {
                if (gameObject->GetName() == "pouvoir en cours")
                {
                    spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
                }
            }
            spriteRenderer_currentPower->SetNextSpriteRect(0);


            InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
            if (input_module->GetControls()->at("ActivER capacitE")->EntryIsPressesd()) {

                if (input_module->GetControls()->at("DRoitE")->EntryIsPressesd() && player->GetComponent<SquareCollider>()->GetCanMoving()["right"]) {
                    speed_dash = window_size.x/100* 100;
                    is_dashing = true;
                    StartClockDurationDash.restart(); // Redémarre le temps du dash
                    StartClockDurationDash.restart(); // Redémarre le temps du dash
                    if (!soundPlayed) {
                        PlaySound();
                        soundDash->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
                        soundPlayed = true; // Marquer que le son a été joué
                    }
                }
                else if (input_module->GetControls()->at("GauchE")->EntryIsPressesd() && player->GetComponent<SquareCollider>()->GetCanMoving()["left"]) {
                    speed_dash = -window_size.x / 100 * 100;
                    is_dashing = true;
                    StartClockDurationDash.restart(); // Redémarre le temps du dash
                    if (!soundPlayed) {
                        PlaySound();
                        soundDash->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
                        soundPlayed = true; // Marquer que le son a été joué
                    }
                }

                SpriteRenderer* spriteRenderer_currentPower = nullptr;
                for (GameObject* const& gameObject : *scene->GetGameObjects())
                {
                    if (gameObject->GetName() == "pouvoir en cours")
                    {
                        spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
                    }
                }
                spriteRenderer_currentPower->SetNextSpriteRect(1);
            }
        }

        // Si le dash est en cours et le cooldown dépasse 5 secondes
        if (is_dashing) {
            player->SetPosition(Maths::Vector2f(player->GetPosition().GetX() + (speed_dash * _delta_time), player->GetPosition().GetY()));
        }
    }
}

void Dash::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
    soundPlayed = false;
    std::vector<GameObject*>* gameObjects = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene()->GetGameObjects();
    IsDashing(_delta_time, gameObjects);
}
