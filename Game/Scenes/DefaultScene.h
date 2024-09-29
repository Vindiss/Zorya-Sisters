#pragma once
#include "Capacity.h"
#include "Player.h"
#include "Scene.h"
#include "SquareCollider.h"
#include "WindowModule.h"
#include <SFML/Audio.hpp>


class Capacity;

class DefaultScene final : public Scene
{
public:

	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();

	DefaultScene(std::vector<Capacity*>* params);

	void FctForCheckpointMenu();

	GameObject* CreateColliderObject(Scene* scene, const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size);

private:
	void SpawnObjectLevel3(std::vector<Capacity*>* params);

	sf::Music music;
};
