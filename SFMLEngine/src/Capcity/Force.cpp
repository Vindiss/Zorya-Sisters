#include "Capacity/Force.h"
#include "Components/Player.h"
#include <Scene.h>
#include <Engine.h>
#include <Modules/SceneModule.h>
#include <iostream>

Force::Force() {
	soundBufferForce = new sf::SoundBuffer;
	//if (!soundBufferForce->loadFromFile("Game_files/Assets/Sons/frottement.wav")) {
	//	std::cout << "erreur de chargement du fichier" << std::endl;
	//}
	soundForce = new sf::Sound;
}

Force::~Force() {
	delete soundBufferForce;
	delete soundForce;
}

void Force::PlaySound() {
	soundForce->setBuffer(*soundBufferForce);
	soundForce->play();
}

void Force::StopSound() {
	soundForce->stop();
}

void Force::DeplaceObject(const float _delta_time, GameObject* player, GameObject* gameObject, std::vector<GameObject*>* gameObjects)
{
	gameObject->GetComponent<SquareCollider>()->SetCanMoving("up", false);
	gameObject->GetComponent<SquareCollider>()->SetCanMoving("down", false);
	float playerX = player->GetPosition().GetX();
	float playerY = player->GetPosition().GetY();
	float gameObjectY = gameObject->GetPosition().GetY();
	float gameObjectX = gameObject->GetPosition().GetX();

	bool collisionbottom = player->GetComponent<SquareCollider>()->CheckCollisionBottom(*player->GetComponent<SquareCollider>(), *gameObject->GetComponent<SquareCollider>());
	bool collisionRight = false;
	bool collisionLeft = false;


	for (GameObject* const& gameObjectbis : *gameObjects)
	{
		if (gameObjectbis->GetType() != PlayerType && gameObject != gameObjectbis && gameObjectbis->GetType() != GameObjectType && gameObjectbis->GetType() != ButtonType)
		{
			bool collisionRighttemp = gameObject->GetComponent<SquareCollider>()->CheckCollisionRight(*gameObject->GetComponent<SquareCollider>(), *gameObjectbis->GetComponent<SquareCollider>());
			bool collisionLefttemp = gameObject->GetComponent<SquareCollider>()->CheckCollisionLeft(*gameObject->GetComponent<SquareCollider>(), *gameObjectbis->GetComponent<SquareCollider>());
			if (collisionLefttemp) {
				collisionLeft = true;
			}
			if (collisionRighttemp) {
				collisionRight = true;
			}
		}
	}
	if ((playerX < gameObjectX))
	{
		if (gameObject->GetComponent<SquareCollider>()->GetCanMoving()["right"] && !collisionbottom && !collisionRight)
		{
			gameObject->SetPosition(Maths::Vector2f(gameObjectX + (player->GetComponent<Player>()->GetSpeed() * _delta_time), gameObjectY));
		}
	}
	else if ((playerX > gameObjectX))
	{
		if (gameObject->GetComponent<SquareCollider>()->GetCanMoving()["left"] && !collisionbottom && !collisionLeft)
		{
			gameObject->SetPosition(Maths::Vector2f(gameObjectX - (player->GetComponent<Player>()->GetSpeed() * _delta_time), gameObjectY));
		}
	}
}

void Force::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input)
{
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	GameObject* player = scene->FindGameObject("Player");

	for (GameObject* const& gameObject : *scene->GetGameObjects())
	{
		if (gameObject->GetType() == PlayerType)
		{
			player = gameObject;
		}
		if (player != nullptr)
		{
			if (gameObject->GetType() != ObjectType::PlayerType && (gameObject->GetType() == MoveType || gameObject->GetType() == DollOffType)){
				if (player->GetComponent<SquareCollider>()->IsColliding(*player->GetComponent<SquareCollider>(), *gameObject->GetComponent<SquareCollider>(), _delta_time))
				{
					DeplaceObject(_delta_time, player, gameObject, scene->GetGameObjects());
					PlaySound();
					soundForce->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
					soundForce->setLoop(true);
					soundPlayed = true; // Marquer que le son a été joué
				}
				
			}
			else
				{
					soundForce->stop();
				}
		}
	}
	
}