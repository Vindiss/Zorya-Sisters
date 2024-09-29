#pragma once
#include "Doll.h"
#include "Engine.h"

#include <SFML/Audio.hpp>

#include "Capacity/DoubleJump.h"

#include "Modules/WindowModule.h"
#include "Modules/SceneModule.h"
#include "Scene.h"

#include "Capacity/Dash.h"
#include "Capacity/DoubleJump.h"
#include "Capacity/Force.h"
#include "Capacity/InversionGravite.h"
#include "Capacity/Invincibilite.h"

class Player : public Component
{
public:
	Player();
	~Player();

	template<typename T>
	T* SetCapacity();

	void SetHp(int new_hp) { hp = new_hp; }
	void SetSpeed(int new_speed) { speed = new_speed; }
	void SetGravity(int _gravity) { gravity = _gravity; }
	void SetSize(int _size) { sizePlayer = _size; }
	void SetCanDoubleJump(bool _can_double_jump) { can_double_jump = _can_double_jump; }
	void SetCapacity(Capacity* _capacity) { capacity = _capacity; }

	void SetPauseEscape(std::function<void()> _function) { pauseEscape = _function; }
	void SetDeathCallback(std::function<void()> _function) { deathCallback = _function; }
	void SetCheckpointCallback(std::function<void()> _function) { checkpointCallback = _function; }

	void setDeathBy(std::string item) { deathBy = item; }
	std::string GetDeathBy() { return deathBy; }

	int GetHp() { return hp; }
	int GetSpeed() { return speed; }
	int GetGravity() { return gravity; }
	int GetSize() { return sizePlayer; }
	Capacity* GetCapacity() { return capacity; }
	bool GetIsJumping() { return is_jumping; }
	bool GetIsCheck() { return is_check; }

	void PlaySound();
	void StopSound();

	bool Dead(std::vector<GameObject*>* gameObjects);
	void Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) override;

private:
	// Get modules
	Engine* engine = Engine::GetInstance();
	ModuleManager* moduleManager = engine->GetModuleManager();

	WindowModule* windowModule = moduleManager->GetModule<WindowModule>();
	sf::Vector2f sizeWindow = windowModule->GetWindowSize();

	SceneModule* sceneModule = moduleManager->GetModule<SceneModule>();

	// // Variables
	int hp = 100;
	bool deathRespawn = false;
	std::string deathBy;

	int speed = sizeWindow.x / 9;
	int gravity = 100;
	float sizePlayer = sizeWindow.y / 22;
	std::string is_moving = "none";

	//Variable move
	bool isWalking = false;

	// Variables jump & switch
	bool can_jump = false;
	bool is_jumping = false;
	sf::Clock jumping_time;
	bool can_double_jump = false;
	bool is_double_jumping = false;

	bool can_switch = false;
	bool is_switching = false;

	bool can_check = false;
	bool is_check = false;

	// GameObject Doll
	int actuall_doll_int = 0;

	GameObject* big_dollOff = nullptr;
	GameObject* mid_dollOff = nullptr;

	Capacity* capacity = nullptr;

	sf::Texture* texture_mid_doll = nullptr;
	sf::Texture* texture_small_doll = nullptr;

	// Checkpoint
	bool copiedSpawn = false;
	std::vector<GameObject*> gameObjectsCheckpoint;
	std::unordered_map<std::string, sf::Texture> texture;

	sf::Color colorBig = sf::Color::Red;
	sf::Color colorMid = sf::Color::Blue;
	sf::Color colorSmall = sf::Color::Green;

	//Sound
	sf::SoundBuffer* soundBufferJump;
	sf::Sound* soundJump;
	sf::SoundBuffer* soundBufferWalk;
	sf::Sound* soundWalk;
	sf::SoundBuffer* soundBufferSwitchDoll;
	sf::Sound* soundSwitchDoll;


	// Functions
	void Move(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, std::vector<GameObject*>* gameObjects );
	void Jump(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, std::vector<GameObject*>* gameObjects, Scene* scene);

	GameObject* CreateDollOff(const ObjectType& _type, std::string _name, Maths::Vector2f _position, sf::Texture* new_texture, Maths::Vector2f _size, Maths::Vector2f _size_sprite);
	void SwitchDoll(std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input, Scene* scene);
	void ReturnCheckpoint(Scene* scene, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input);
	void TPFinDuLevel(Scene* scene, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input);

	void PauseMenu(std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input);

	std::function<void()> pauseEscape;
	std::function<void()> deathCallback;
	std::function<void()> checkpointCallback;
};
