#include "Capacity/InversionGravite.h"
#include <Engine.h>
#include <Modules/SceneModule.h>
#include <Components/Player.h>
#include <iostream>

//Sound
InversionGravite::InversionGravite() {
	soundBufferInversionGravite = new sf::SoundBuffer;
	if (!soundBufferInversionGravite->loadFromFile("Game_files/Assets/Sons/inversion_gravite.wav")) {
		std::cout << "erreur de chargement du fichier" << std::endl;
	}
	soundInversionGravite = new sf::Sound;
}

InversionGravite::~InversionGravite() {
	delete soundBufferInversionGravite;
	delete soundInversionGravite;
}

void InversionGravite::PlaySound() {
	soundInversionGravite->setBuffer(*soundBufferInversionGravite);
	soundInversionGravite->play();
}

void InversionGravite::StopSound() {
	soundInversionGravite->stop();
}

void InversionGravite::GraviteInversion(GameObject* player, const float _delta_time, Scene* scene)
{
	if (player->GetComponent<Player>()->GetIsCheck())
	{
		inversion = false;
	}

	if (count == 0)
	{
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		if (input_module->GetControls()->at("ActivER capacitE")->EntryIsPressesd())
		{
			if (!player->GetComponent<SquareCollider>()->GetCanMoving()["down"])
			{
				inversionClock.restart();
				inversion = true;
				count = 1;


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

			if (!soundPlayed) {
				PlaySound();
				soundInversionGravite->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
				soundPlayed = true; // Marquer que le son a été joué
			}
		}
	}
	if (inversion == true && player->GetComponent<SquareCollider>()->GetCanMoving()["up"])
	{
		if (inversionClock.getElapsedTime().asSeconds() <= 1) {
			GetCapacityOwner()->GetComponent<Player>()->SetGravity(-75);
		}
		else if (inversionClock.getElapsedTime().asSeconds() <= 2) {
			GetCapacityOwner()->GetComponent<Player>()->SetGravity(-150);
		}
		else if (inversionClock.getElapsedTime().asSeconds() <= 3) {
			GetCapacityOwner()->GetComponent<Player>()->SetGravity(-200);
		}
		else {
			inversion = false;
			GetCapacityOwner()->GetComponent<Player>()->SetGravity(100);
		}
	}
	else if (!inversion)
	{
		GetCapacityOwner()->GetComponent<Player>()->SetGravity(100);
	}
}

void InversionGravite::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input)
{
	soundPlayed = false;

	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	GameObject* player = nullptr;

	for (GameObject* const& gameObject : *scene->GetGameObjects())
	{
		if (gameObject->GetType() == PlayerType)
		{
			player = gameObject;
		}
		if (player != nullptr)
		{
			GraviteInversion(player, _delta_time, scene);
		}
	}
	
}