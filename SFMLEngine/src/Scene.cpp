#include "Scene.h"
#include "Capacity.h"
#include "Modules/SceneModule.h" 
#include "Engine.h"

#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "Modules/WindowModule.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <Components/Player.h>
#include <Components/Interactive.h>


class Capacity;

Scene::Scene(const std::string& _name, std::function<void()> _callback)
{
	name = _name;
	callBack = _callback;
	//font_scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetFont();

	Engine* engine = Engine::GetInstance();
	ModuleManager* moduleManager = engine->GetModuleManager();
	SceneModule* sceneModule = moduleManager->GetModule<SceneModule>();
	font_scene = sceneModule->GetFont();
}

Scene::~Scene() {
	for (int i = 0; i < gameObjects.size(); i++)
	{
		DestroyGameObject(gameObjects[i]);
	}
	texture.clear();
}

void Scene::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) 
{
	if (name == "IntroScene")
	{
		callBack();
	}

	if (name == "TransitionScene" && !gameLoaded)
	{
		SetGameLoaded(true);
		callBack();
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->GetType() == MoveType || gameObjects[i]->GetType() == DollOffType)
		{
			gameObjects[i]->GetComponent<SquareCollider>()->SetCanMoving("up", true);
			gameObjects[i]->GetComponent<SquareCollider>()->SetCanMoving("down", true);
			gameObjects[i]->GetComponent<SquareCollider>()->SetCanMoving("left", true);
			gameObjects[i]->GetComponent<SquareCollider>()->SetCanMoving("right", true);

			for (int j = 0; j < gameObjects.size(); j++)
			{
				if (gameObjects[i] != gameObjects[j] && gameObjects[j]->GetType() != GameObjectType && gameObjects[j]->GetType() != ButtonType)
				{
					gameObjects[i]->GetComponent<SquareCollider>()->IsColliding(*gameObjects[i]->GetComponent<SquareCollider>(), *gameObjects[j]->GetComponent<SquareCollider>(), _delta_time);
				}
			}
			if (gameObjects[i]->GetComponent<SquareCollider>()->GetCanMoving()["down"])
			{
				gameObjects[i]->SetPosition(Maths::Vector2f(gameObjects[i]->GetPosition().GetX(), gameObjects[i]->GetPosition().GetY() + (gravity * _delta_time)));
			}
		}
		gameObjects[i]->Update(_delta_time, pressed_input);
	}
}



void Scene::Render(sf::RenderWindow* _window) const	
{

	if (background != nullptr)
	{
		_window->draw(*background);
	}
	
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Render(_window);
	}
	//for (GameObject* const& game_object : gameObjects)
	//{
	//	game_object->Render(_window);
	//}
}


std::string Scene::GetName() const {
	return name;
}

GameObject* Scene::CreateGameObject(const ObjectType& _type, std::string _name)
{
	GameObject* const game_object = new GameObject();
	game_object->SetType(_type);
	game_object->SetName(_name);
	gameObjects.push_back(game_object);
	return game_object;
}

void Scene::DestroyGameObject(const GameObject* _game_object)
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (*it == _game_object)
		{
			gameObjects.erase(it);
			delete _game_object;
			return;
		}
	}
}

GameObject* Scene::FindGameObject(const std::string& _name) const
{
	for (GameObject* const& game_object : gameObjects)
	{
		if (game_object->GetName() == _name)
		{
			return game_object;
		}
	}
	return nullptr;
}

int Scene::SetTexture(std::string nom_texture, std::string chemin_fichier)
{
	sf::Texture new_texture;

	if (!new_texture.loadFromFile(chemin_fichier))
	{
		return EXIT_FAILURE;
	}
	new_texture.setSmooth(true);
	texture[nom_texture] = new_texture;

	return EXIT_SUCCESS;
}

sf::Texture* Scene::GetTextureByName(std::string _name) {
	return &texture[_name];
}

void Scene::SetBackground(std::string nom_texture) {
	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();

	background = new sf::Sprite();
	// Redimensionner le sprite pour qu'il remplisse la fen?tre
	background->setTexture(texture[nom_texture]);
	background->setScale((float)window_size.x / background->getTexture()->getSize().x, (float)window_size.y / background->getTexture()->getSize().y);
}

GameObject* Scene::CreateSpriteButton(const ObjectType& _type, std::string _name, Maths::Vector2f _position, const sf::Color _color, sf::Color _outlineColor, sf::Color _hoverColor, sf::Color _clickColor, Maths::Vector2f size, std::function<void()> _callback, Capacity* _object, std::string nom_texture)
{
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetColor(_color);
	shape_renderer->SetDefaultColor(_color);
	shape_renderer->SetOutlineColor(_outlineColor);
	shape_renderer->SetHoverColor(_hoverColor);
	shape_renderer->SetClickColor(_clickColor);
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	sprite_renderer->SetSprite(&texture[nom_texture], size);

	Button* button = game_object->CreateComponent<Button>();
	button->SetObject(_object);
	button->SetCallback(_callback);

	return game_object;
}
sf::Vector2i GetMousePosition(sf::RenderWindow& window) {
	return sf::Mouse::getPosition(window);
}

GameObject* Scene::CreateSliderButton(const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f size, std::function<void()> _callback, Capacity* _object, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space)
{
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}

	Button* button = game_object->CreateComponent<Button>();
	button->SetObject(_object);


	return game_object;
}


GameObject* Scene::CreateSpriteButton_forMainMenu(const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f size, std::function<void()> _callback, Capacity* _object, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space)
{
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}

	Button* button = game_object->CreateComponent<Button>();
	button->SetObject(_object);
	button->SetCallback(_callback);

	return game_object;
}

GameObject* Scene::CreateButtonWithText(const ObjectType& _type, std::string _name, Maths::Vector2f _position, const sf::Color _color, sf::Color _outlineColor, sf::Color _hoverColor, sf::Color _clickColor, Maths::Vector2u size, std::function<void()> _callback, Capacity* _object, std::string button_text, sf::Color text_color, int _caractere_size)
{
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetColor(_color);
	shape_renderer->SetDefaultColor(_color);
	shape_renderer->SetOutlineColor(_outlineColor);
	shape_renderer->SetHoverColor(_hoverColor);
	shape_renderer->SetClickColor(_clickColor);
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	TextRenderer* text_renderer = game_object->CreateComponent<TextRenderer>();
	text_renderer->SetString(button_text);
	text_renderer->SetColor(text_color);
	text_renderer->SetSize(Maths::Vector2f(size.x, size.y));
	text_renderer->SetCaracterSize(_caractere_size);

	Button* button = game_object->CreateComponent<Button>();
	button->SetObject(_object);
	button->SetCallback(_callback);

	return game_object;
}

GameObject* Scene::CreateSpriteButtonWithText(const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f size, std::function<void()> _callback, Capacity* _object, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space, std::string button_text, sf::Color text_color, int _caractere_size)
{
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}

	TextRenderer* text_renderer = game_object->CreateComponent<TextRenderer>();
	text_renderer->SetString(button_text);
	text_renderer->SetColor(text_color);
	text_renderer->SetSize(Maths::Vector2f(size.x, size.y));
	text_renderer->SetCaracterSize(_caractere_size);

	Button* button = game_object->CreateComponent<Button>();
	button->SetObject(_object);
	button->SetCallback(_callback);

	return game_object;
}

GameObject* Scene::CreateText(const ObjectType& _type, std::string _name, Maths::Vector2f _position, const sf::Color _color, Maths::Vector2u size, int _caractere_size) {
	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	TextRenderer* text_renderer = game_object->CreateComponent<TextRenderer>();
	text_renderer->SetColor(_color);
	text_renderer->SetSize(Maths::Vector2f(size.x, size.y));
	text_renderer->SetCaracterSize(_caractere_size);

	return game_object;
}

GameObject* Scene::CreateShapeAlone(const ObjectType& _type, std::string _name, Maths::Vector2f _position, const sf::Color _color, sf::Vector2f size) {
	GameObject* game_object = CreateGameObject(_type, _name); 
	game_object->SetPosition(_position); 

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetColor(_color);
	shape_renderer->SetSize(Maths::Vector2f(size.x, size.y));

	return game_object;
}


GameObject* Scene::CreateSpriteObject(Scene* scene, const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space) {
	GameObject* game_object = scene->CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	SquareCollider* squareCollider = game_object->CreateComponent<SquareCollider>();
	squareCollider->SetWidth(_size.x);
	squareCollider->SetHeight(_size.y);

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], _size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], _size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}

	return game_object;
}

GameObject* Scene::CreateInteractiveObject(Scene* scene, const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space, GameObject* _object) {
	GameObject* game_object = scene->CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	SquareCollider* squareCollider = game_object->CreateComponent<SquareCollider>();
	squareCollider->SetWidth(_size.x);
	squareCollider->SetHeight(_size.y);

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], _size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], _size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}

	Interactive* interactive = game_object->CreateComponent<Interactive>();
	interactive->SetObject(_object);

	return game_object;
}


GameObject* Scene::CreateOnlySprite(Scene* scene, const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space) {
	GameObject* game_object = scene->CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], _size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], _size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
	}


	return game_object;
}

GameObject* Scene::CreatePlayer(const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size, std::string nom_texture, Maths::Vector2f _sprite_size, Maths::Vector2f sprite_space) {

	GameObject* game_object = CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	SquareCollider* square_collider = game_object->CreateComponent<SquareCollider>();
	square_collider->SetWidth(_size.x * 0.69);
	square_collider->SetHeight(_size.y * 0.75);
	square_collider->SetSpecialPosition(Maths::Vector2f(_size.x * 0.148, _size.y * 0.248));

	SpriteRenderer* sprite_renderer = game_object->CreateComponent<SpriteRenderer>();
	if (sprite_space.x == 0 && sprite_space.y == 0) {
		sprite_renderer->SetSprite(&texture[nom_texture], _size);
	}
	else {
		sprite_renderer->SetSpriteRect(&texture[nom_texture], _size, _sprite_size, Maths::Vector2f(0, 0), sprite_space);
		sprite_renderer->SetSpriteFirstPosition(Maths::Vector2f(841, 0));
	}

	Player* player = game_object->CreateComponent<Player>();

	if (GetBigCapacity()->GetName() == "INVERSION DE LA GRaVITE") {
		InversionGravite* capacity = player->SetCapacity<InversionGravite>();
		capacity->SetName("INVERSION DE LA GRaVITE");
		capacity->SetCapacityOwner(game_object);
	}
	else if (GetBigCapacity()->GetName() == "INVINCIbILITE") {
		Invincibilite* capacity = player->SetCapacity<Invincibilite>();
		capacity->SetName("INVINCIbILITE");
		capacity->SetCapacityOwner(game_object);
	}
	else if (GetBigCapacity()->GetName() == "DOUbLE SaUT") {
		DoubleJump* capacity = player->SetCapacity<DoubleJump>();
		capacity->SetName("DOUbLE SaUT");
		capacity->SetCapacityOwner(game_object);
		capacity->SetDoubleJump(true);
	}
	else if (GetBigCapacity()->GetName() == "DaSH") {
		Dash* capacity = player->SetCapacity<Dash>();
		capacity->SetName("DaSH");
		capacity->SetCapacityOwner(game_object);
	}
	else if (GetBigCapacity()->GetName() == "fORCE") {
		Force* capacity = player->SetCapacity<Force>();
		capacity->SetName("fORCE");
		capacity->SetCapacityOwner(game_object);
	}

	return game_object;
}
