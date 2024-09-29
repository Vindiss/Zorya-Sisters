#include "Components/Player.h"
#include <iostream>
#include "Components/RectangleShapeRenderer.h"
#include <Components/SquareCollider.h>
#include <Scene.h>
#include <Modules/SceneModule.h>
#include "Components/ObjectType.h"
#include "Modules/WindowModule.h"

#include <Capacity/Force.h>
#include <Capacity/Invincibilite.h>
#include <Capacity/InversionGravite.h>

#include "Capacity/Dash.h"


//Sound
Player::Player() {
	soundBufferJump = new sf::SoundBuffer;
	if (!soundBufferJump->loadFromFile("Game_files/Assets/Sons/Saut.wav")) {
		std::cout << "erreur de chargement du fichier" << std::endl;
	}
	soundJump = new sf::Sound;

	soundBufferWalk = new sf::SoundBuffer;
	if (!soundBufferWalk->loadFromFile("Game_files/Assets/Sons/deplacement.wav")) {
		std::cout << "erreur de chargement du fichier" << std::endl;
	}
	soundWalk = new sf::Sound;

	soundBufferSwitchDoll = new sf::SoundBuffer;
	if (!soundBufferSwitchDoll->loadFromFile("Game_files/Assets/Sons/changement_poupee.wav")) {
		std::cout << "erreur de chargement du fichier" << std::endl;
	}
	soundSwitchDoll = new sf::Sound;
}

Player::~Player() {
	delete soundBufferJump;
	delete soundJump;
	delete soundBufferWalk;
	delete soundWalk;
	delete soundBufferSwitchDoll;
	delete soundSwitchDoll;
}

void Player::PlaySound() {
	soundJump->setBuffer(*soundBufferJump);
	soundJump->play();
	soundWalk->setBuffer(*soundBufferWalk);
	soundWalk->play();
	soundSwitchDoll->setBuffer(*soundBufferSwitchDoll);
	soundSwitchDoll->play();
}

void Player::StopSound() {
	soundJump->stop();
	soundWalk->stop();
	soundSwitchDoll->stop();
}

void Player::Move(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, std::vector<GameObject*>* gameObjects){
	Maths::Vector2f start_position = GetOwner()->GetPosition();
	if (!isWalking)
	{
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		for (const auto& input : *pressed_input) {
			if (input.first == input_module->GetControls()->at("DRoitE")->GetEntry() && input.second == true && GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["right"]) {
				GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX() + (speed * _delta_time), GetOwner()->GetPosition().GetY()));
				isWalking = true; // Activation du son si D est enfoncée
			}
			else if (input.first == input_module->GetControls()->at("GauchE")->GetEntry() && input.second == true && GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["left"]) {
				GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX() - (speed * _delta_time), GetOwner()->GetPosition().GetY()));
				isWalking = true; // Activation du son si Q est enfoncée
			}
			if (isWalking) {
				// Son Déplacement en boucle
				soundWalk->setBuffer(*soundBufferWalk);
				soundWalk->setLoop(true); // Jouer en boucle
				soundWalk->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
				soundWalk->play();
				/*std::cout << "son de marche" << std::endl;*/
			}
		}
		soundWalk->stop();
	}
	
	
	if (start_position != GetOwner()->GetPosition()) {
		if (start_position.x < GetOwner()->GetPosition().x) {
			GetOwner()->GetComponent<SpriteRenderer>()->SetSpriteFirstPosition(Maths::Vector2f(GetOwner()->GetComponent<SpriteRenderer>()->GetSpriteSize().x * 2 + 1, 0));
		}
		else {
			GetOwner()->GetComponent<SpriteRenderer>()->SetSpriteFirstPosition(Maths::Vector2f(GetOwner()->GetComponent<SpriteRenderer>()->GetSpriteSize().x * 3 + 1, 0));
		}
		GetOwner()->SetIsAnimated(true);
		GetOwner()->SetReverseAnimation(false);
		GetOwner()->SetNbAnimationSprite(3);
	}
	else {
		GetOwner()->SetReverseAnimation(true);
	}
}

void Player::Jump(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, std::vector<GameObject*>* gameObjects, Scene* scene) {
	if (!GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["down"] && is_jumping) {
		is_jumping = false;
		is_double_jumping = false;


		// changer le sprite si pouvoir
		if (capacity->GetName() == "DoubleJump")
		{
			SpriteRenderer* spriteRenderer_currentPower = nullptr;
			for (GameObject* const& gameObject : *scene->GetGameObjects())
			{
				if (gameObject->GetName() == "pouvoir en cours")
				{
					spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
				}
			}
			spriteRenderer_currentPower->SetNextSpriteRect(0);
		}
	}
	if (is_jumping && can_double_jump && !is_double_jumping) {
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		for (const auto& input : *pressed_input) {
			if (input.first == input_module->GetControls()->at("SautER")->GetEntry() && input.second == true) {
				is_jumping = true;
				is_double_jumping = true;
				jumping_time.restart();

				can_switch = false;

				// changer le sprite si pouvoir
				if (capacity->GetName() == "DoubleJump")
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
			}
		}
	}
	if (can_jump && !is_jumping) {
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		for (auto& input : *pressed_input) {
			if (input.first == input_module->GetControls()->at("SautER")->GetEntry() && input.second == true) {
				is_jumping = true;
				jumping_time.restart();

				can_switch = false;

				input.second = false;
			}
		}
	}

	if (!GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["up"]) {
		is_jumping = false;
	}

	if (is_jumping && GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["up"]) {
		if (GetOwner()->GetComponent<SpriteRenderer>()->GetActualSpriteInt() != 0 && GetOwner()->GetComponent<SpriteRenderer>()->GetSpriteFirstPosition() != Maths::Vector2f(0,0)) {
			GetOwner()->SetReverseAnimation(true);
		}
		else {
			GetOwner()->SetReverseAnimation(false);
			GetOwner()->SetIsAnimated(true);
			GetOwner()->SetNbAnimationSprite(7);
			GetOwner()->GetComponent<SpriteRenderer>()->SetSpriteFirstPosition(Maths::Vector2f(0, 0));

		}
		if (jumping_time.getElapsedTime().asSeconds() <= 0.01) {
			//Son Jump
			soundJump->setBuffer(*soundBufferJump);
			soundJump->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
			soundJump->play();
		}
		if (jumping_time.getElapsedTime().asSeconds() <= 0.4) {
		GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() - (sizeWindow.y * 0.3 * _delta_time)));
		}
		else if (jumping_time.getElapsedTime().asSeconds() <= 0.5) {
		GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() - (sizeWindow.y * 0 * _delta_time)));
		}
		else{
		GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() + (sizeWindow.y * 0.3 * _delta_time)));
		can_jump = false;
		}
	}

//if (GetOwner()->GetPosition().GetY() + 200 <= sizeWindow.y && !is_jumping && GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["down"]) {
//	GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() + (sizeWindow.y * 0.1 * _delta_time)));
//}

}

GameObject* Player::CreateDollOff(const ObjectType& _type, std::string _name, Maths::Vector2f _position, sf::Texture* texture, Maths::Vector2f _size, Maths::Vector2f _size_sprite) {
	Scene* scene = sceneModule->GetScene("DefaultScene");

	GameObject* game_object = scene->CreateGameObject(_type, _name);
	game_object->SetPosition(_position);


	SquareCollider* square_collider = game_object->CreateComponent<SquareCollider>();
	square_collider->SetWidth(_size.x);
	square_collider->SetHeight(_size.y);

	SpriteRenderer* sprite = game_object->CreateComponent<SpriteRenderer>();
	sprite->SetSprite(texture, _size);

	DollOff* dollOff = game_object->CreateComponent<DollOff>();
	dollOff->setPosition(_position);

	return game_object;
}

void Player::SwitchDoll(std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, Scene* scene) {
	if (can_switch && !is_switching)
	{
		auto it = pressed_input->begin();
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();

		while (it != pressed_input->end()) {
			const auto& input = *it;

			if (input.first == input_module->GetControls()->at("PassER a la pRochainE poupEE")->GetEntry() && input.second == true) {
				is_switching = true;
				// Effacer l'élément du vecteur
				it = pressed_input->erase(it);

				//Son changement de poupée
				soundSwitchDoll->setBuffer(*soundBufferSwitchDoll);
				soundSwitchDoll->setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
				soundSwitchDoll->play();
			}
			else {
				++it;
			}
		}
	}

	if (is_switching)
	{
		can_switch = false;
		can_jump = false;
		can_check = false;

		SpriteRenderer* spriteRenderer_currentPower = nullptr;
		for (GameObject* const& gameObject : *scene->GetGameObjects()) 
		{ 
			if (gameObject->GetName() == "pouvoir en cours")
			{
				spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
			}
		} 
	

		if (actuall_doll_int == 0)
		{
			Maths::Vector2f position = GetOwner()->GetPosition();
			Maths::Vector2f size = Maths::Vector2f(GetOwner()->GetComponent<SquareCollider>()->GetWidth(), GetOwner()->GetComponent<SquareCollider>()->GetHeight());

			big_dollOff = CreateDollOff(DollOffType, "big_doll_off", position, scene->GetTextureByName("texture_zarya_gris"), size, Maths::Vector2f(420, 654));
			GetOwner()->SetPosition(Maths::Vector2f(position.GetX(), position.GetY() - sizePlayer * 1.5));
			GetOwner()->GetComponent<SpriteRenderer>()->SetSpriteRect(scene->GetTextureByName("texture_zvezda"), Maths::Vector2f((sizeWindow.x / 33), (((sizeWindow.x / 33) * 554) / 345)), Maths::Vector2f(345, 554), Maths::Vector2f(0,0), Maths::Vector2f(0, 1));
			GetOwner()->GetComponent<SquareCollider>()->SetWidth(sizeWindow.x / 33 * 0.75);
			GetOwner()->GetComponent<SquareCollider>()->SetHeight((((sizeWindow.x / 33) * 554) / 345) * 0.75);
			GetOwner()->GetComponent<SquareCollider>()->SetSpecialPosition(Maths::Vector2f(sizeWindow.x / 33 * 0.113, (((sizeWindow.x / 33) * 554) / 345) * 0.248));
			Capacity* capacity_for_mid_doll = scene->GetMidCapacity();

			delete capacity;
			capacity = nullptr;
			if (capacity_for_mid_doll->GetName() == "INVERSION DE LA GRaVITE") {
				InversionGravite* new_capacity = SetCapacity<InversionGravite>();
				new_capacity->SetName("INVERSION DE LA GRaVITE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_gravity"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_mid_doll->GetName() == "INVINCIbILITE") {
				Invincibilite* new_capacity = SetCapacity<Invincibilite>();
				new_capacity->SetName("INVINCIbILITE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_invincibilite"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_mid_doll->GetName() == "DOUbLE SaUT") {
				DoubleJump* new_capacity = SetCapacity<DoubleJump>();
				new_capacity->SetName("DOUbLE SaUT");
				new_capacity->SetCapacityOwner(GetOwner());
				new_capacity->SetDoubleJump(true);
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_double_jump"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_mid_doll->GetName() == "DaSH") {
				Dash* new_capacity = SetCapacity<Dash>();
				new_capacity->SetName("DaSH");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_dash"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_mid_doll->GetName() == "fORCE") {
				Force* new_capacity = SetCapacity<Force>();
				new_capacity->SetName("fORCE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_force"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}

			//// Création du Checkpoint
			// Copie des GameObjects
			gameObjectsCheckpoint.clear();

			for (GameObject* const& gameObject : *scene->GetGameObjects())
			{
				GameObject* gameObjectTemp = new GameObject(*gameObject);
				gameObjectsCheckpoint.push_back(gameObjectTemp);
			}

			////




			actuall_doll_int++;
		}
		else if (actuall_doll_int == 1)
		{
			Maths::Vector2f position = GetOwner()->GetPosition();
			Maths::Vector2f size = Maths::Vector2f(GetOwner()->GetComponent<SquareCollider>()->GetWidth(), GetOwner()->GetComponent<SquareCollider>()->GetHeight());

			mid_dollOff = CreateDollOff(DollOffType, "mid_doll_off", position, scene->GetTextureByName("texture_zvezda_gris"), size, Maths::Vector2f(345, 554));
			GetOwner()->SetPosition(Maths::Vector2f(position.GetX(), position.GetY() - sizePlayer * 1.5));
			GetOwner()->GetComponent<SpriteRenderer>()->SetSpriteRect(scene->GetTextureByName("texture_zwezda"), Maths::Vector2f((sizeWindow.x / 40), (((sizeWindow.x / 40) * 411) / 274)), Maths::Vector2f(274, 411), Maths::Vector2f(0, 0), Maths::Vector2f(0, 1));
			GetOwner()->GetComponent<SquareCollider>()->SetWidth(sizeWindow.x / 40 * 0.76);
			GetOwner()->GetComponent<SquareCollider>()->SetHeight(((sizeWindow.x / 40) * 411) / 274 * 0.79);
			GetOwner()->GetComponent<SquareCollider>()->SetSpecialPosition(Maths::Vector2f(sizeWindow.x / 33 * 0.102, (((sizeWindow.x / 33) * 554) / 345) * 0.206));
			Capacity* capacity_for_small_doll = scene->GetSmallCapacity();

			delete capacity;
			capacity = nullptr;

			if (capacity_for_small_doll->GetName() == "INVERSION DE LA GRaVITE") {
				InversionGravite* new_capacity = SetCapacity<InversionGravite>();
				new_capacity->SetName("INVERSION DE LA GRaVITE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_gravity"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_small_doll->GetName() == "INVINCIbILITE") {
				Invincibilite* new_capacity = SetCapacity<Invincibilite>();
				new_capacity->SetName("INVINCIbILITE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_invincibilite"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_small_doll->GetName() == "DOUbLE SaUT") {
				DoubleJump* new_capacity = SetCapacity<DoubleJump>();
				new_capacity->SetName("DOUbLE SaUT");
				new_capacity->SetCapacityOwner(GetOwner());
				new_capacity->SetDoubleJump(true);
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_double_jump"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_small_doll->GetName() == "DaSH") {
				Dash* new_capacity = SetCapacity<Dash>();
				new_capacity->SetName("DaSH");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_dash"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}
			else if (capacity_for_small_doll->GetName() == "fORCE") {
				Force* new_capacity = SetCapacity<Force>();
				new_capacity->SetName("fORCE");
				new_capacity->SetCapacityOwner(GetOwner());
				capacity = new_capacity;

				spriteRenderer_currentPower->SetSpriteRect(scene->GetTextureByName("texture_force"), Maths::Vector2f(sizeWindow.x / 20, ((((sizeWindow.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
			}

			//// Création du Checkpoint
			// Copie des GameObjects
			gameObjectsCheckpoint.clear();

			for (GameObject* const& gameObject : *scene->GetGameObjects())
			{
				GameObject* gameObjectTemp = new GameObject(*gameObject);
				gameObjectsCheckpoint.push_back(gameObjectTemp);
			}

			////

			actuall_doll_int++;
		}
		is_switching = false;
	}
}

void Player::TPFinDuLevel(Scene* scene, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	auto it = pressed_input->begin();

	while (it != pressed_input->end()) {
		const auto& input = *it;

		if (input.first == 12 && input.second == true) {

			for (GameObject* const& gameObject : *scene->GetGameObjects())
			{
				if (gameObject->GetType() == PlayerType)
				{
					gameObject->SetPosition(Maths::Vector2f(sizeWindow.x / 100 * 96.5104, sizeWindow.y / 100 * 87.3333));
				}
			}


			// Effacer l'élément du vecteur
			it = pressed_input->erase(it);
		}
		else {
			++it;
		}
	}
}

void Player::ReturnCheckpoint(Scene* scene, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	if (can_check && !is_check)
	{
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		auto it = pressed_input->begin();

		while (it != pressed_input->end()) {
			const auto& input = *it;

			if (input.first == input_module->GetControls()->at("REvEniR au chEcKpoint")->GetEntry() && input.second == true) {
				is_check = true;
				// Effacer l'élément du vecteur
				it = pressed_input->erase(it);
			}
			else {
				++it;
			}
		}
	}

	if (is_check)
	{
		can_switch = false;
		can_jump = false;
		can_check = false;

		Maths::Vector2f position = GetOwner()->GetPosition();

		std::string nameScene = scene->GetName();
			
		std::vector<GameObject*>* gameObjects = scene->GetGameObjects();

		for (int i = 0; i < gameObjects->size(); i++)
		{
			if (i >= gameObjectsCheckpoint.size())
			{
				scene->DestroyGameObject((*gameObjects)[i]);
			}
			else
			{
				*(*gameObjects)[i] = *gameObjectsCheckpoint[i];
			}
		}
		
		if (capacity->GetName() == "InversionGravite")
		{
			InversionGravite* new_capacity = SetCapacity<InversionGravite>(); 
			new_capacity->SetName("InversionGravite");
			new_capacity->SetCapacityOwner(GetOwner());
			capacity = new_capacity;

			

			SpriteRenderer* spriteRenderer_currentPower = nullptr;
			for (GameObject* const& gameObject : *scene->GetGameObjects())
			{
				if (gameObject->GetName() == "pouvoir en cours")
				{
					spriteRenderer_currentPower = gameObject->GetComponent<SpriteRenderer>();
				}
			}
			spriteRenderer_currentPower->SetNextSpriteRect(0);
		}

		checkpointCallback();
		scene->GetParams()->clear();
		scene->GetParams()->push_back((scene->GetBigCapacity()));
		scene->GetParams()->push_back((scene->GetMidCapacity()));
		scene->GetParams()->push_back((scene->GetSmallCapacity()));

		is_check = false;
	}
}

void Player::PauseMenu(std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {

	auto it = pressed_input->begin();

	while (it != pressed_input->end()) {
		const auto& input = *it;

		if (input.first == 36 && input.second == true) {
			
			pauseEscape();
			// Effacer l'élément du vecteur
			it = pressed_input->erase(it);
		}
		else {
			++it;
		}
	}
}

bool Player::Dead(std::vector<GameObject*>* gameObjects)
{
	if (hp <= 0)
	{
		/*std::cout << "T'es mort!!" << std::endl;
		GetOwner()->RemoveComponent(GetOwner()->GetComponent<Player>());
		GetOwner()->RemoveComponent(GetOwner()->GetComponent<SpriteRenderer>());
		GetOwner()->RemoveComponent(GetOwner()->GetComponent<SquareCollider>());
		for (auto it = gameObjects->begin(); it != gameObjects->end(); ++it)
		{
			if (*it == GetOwner())
			{
				delete GetOwner();
				gameObjects->erase(it);
				return true;
			}
		}*/
		deathCallback();
		deathRespawn = true;
	}
	else
	{
		return false;
	}
}


void Player::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	isWalking = false;

	if (!copiedSpawn)
	{
		for (GameObject* const& gameObject : *scene->GetGameObjects())
		{
			GameObject* gameObjectTemp = new GameObject(*gameObject); 
			gameObjectsCheckpoint.push_back(gameObjectTemp); 
		}

		copiedSpawn = true;
	}

	if (deathRespawn)
	{
		Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
		std::vector<GameObject*>* gameObjects = scene->GetGameObjects();

		for (int i = 0; i < gameObjects->size(); i++)
		{
			if (i >= gameObjectsCheckpoint.size())
			{
				scene->DestroyGameObject((*gameObjects)[i]);
			}
			else
			{
				*(*gameObjects)[i] = *gameObjectsCheckpoint[i];
			}
		}


		hp = 100;
		gravity = 100;
		deathRespawn = false;

		checkpointCallback();
		scene->GetParams()->clear();
		scene->GetParams()->push_back((scene->GetBigCapacity()));
		scene->GetParams()->push_back((scene->GetMidCapacity()));
		scene->GetParams()->push_back((scene->GetSmallCapacity()));
	}
	

	GetCapacity()->Update(_delta_time, pressed_input);

	GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("up", true);
	GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("down", true);
	GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("left", true);
	GetOwner()->GetComponent<SquareCollider>()->SetCanMoving("right", true);
	for (GameObject* const& gameObject : *scene->GetGameObjects())
	{
		if (gameObject->GetType() != ObjectType::PlayerType && gameObject->GetType() != ObjectType::GameObjectType && gameObject->GetType() != ObjectType::ButtonType) {
			GetOwner()->GetComponent<SquareCollider>()->IsColliding(*GetOwner()->GetComponent<SquareCollider>(), *gameObject->GetComponent<SquareCollider>() , _delta_time);
		}
	}
	Move(_delta_time, pressed_input, scene->GetGameObjects());
	Jump(_delta_time, pressed_input, scene->GetGameObjects(), scene);
	SwitchDoll(pressed_input, scene);
	ReturnCheckpoint(scene, pressed_input); 
	TPFinDuLevel(scene, pressed_input);
	PauseMenu(pressed_input);
	Dead(scene->GetGameObjects());

	for (GameObject* const& gameObject : *scene->GetGameObjects())
	{
		if (gameObject->GetType() == ObjectType::SwitchType && SquareCollider::IsColliding(*GetOwner()->GetComponent<SquareCollider>(), *gameObject->GetComponent<SquareCollider>(), _delta_time))
		{
			GameObject* switchObject = GetOwner();
			if (switchObject)
			{
				switchObject->SetSwitchOn(true);
				std::cout << switchObject->GetSwitchOn() << std::endl;
			}
		}
	}


	can_check = true;
	if (GetOwner()->GetPosition().GetX() <= 0) {
		GetOwner()->SetPosition(Maths::Vector2f(0, GetOwner()->GetPosition().GetY()));
	}
	if (GetOwner()->GetPosition().GetX() + sizePlayer >= sizeWindow.x) {
		GetOwner()->SetPosition(Maths::Vector2f(sizeWindow.x - sizePlayer, GetOwner()->GetPosition().GetY()));
	}

	bool temp = false;
	if (gravity < 0)
	{
		temp = GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["up"];
		if (GetOwner()->GetPosition().GetY() + sizePlayer <= sizeWindow.y && temp) {
			GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() + (gravity * _delta_time)));
			can_switch = false;
			can_jump = false;
		}
	}
	else
	{
		temp = GetOwner()->GetComponent<SquareCollider>()->GetCanMoving()["down"];
		if (GetOwner()->GetPosition().GetY() + sizePlayer <= sizeWindow.y && temp) {
			GetOwner()->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().GetX(), GetOwner()->GetPosition().GetY() + (gravity * _delta_time)));
			can_switch = false;
			can_jump = false;
		}
		else {
			can_jump = true;
			if (actuall_doll_int != 2)
			{
				can_switch = true;
			}
		}
	}
}

template<typename T>
T* Player::SetCapacity()
{
	T* _capacity = new T();
	capacity = _capacity;
	return _capacity;
}
