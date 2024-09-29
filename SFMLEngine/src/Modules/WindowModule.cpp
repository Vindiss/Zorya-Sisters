#include "Modules/WindowModule.h"
#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <iostream>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "Engine.h"
#include "Modules/SceneModule.h"
#include "Scene.h"
#include "Modules/InputModule.h"

void WindowModule::Init()
{
	Module::Init();

	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "SFML Engine", sf::Style::Fullscreen);
}

void WindowModule::Start()
{
	Module::Start();
}

sf::Vector2i WindowModule::GetMousePosition() {
	sf::Vector2i mousePos(-1, -1); // Valeurs par défaut si aucun clic droit n'a été détecté

	mousePos = sf::Mouse::getPosition(*window);

	return mousePos;
}
void WindowModule::Update()
{
	Module::Update();
	Scene* scene = moduleManager->GetModule<SceneModule>()->GetMainScene();
	GameObject* change_entry = nullptr;
	std::vector<GameObject*>* gameObjects = scene->GetGameObjects();
	for (int i = 0; i < gameObjects->size(); i++) {
		GameObject* gameobject = (*gameObjects)[i];
		if (gameobject->GetType() == EntryButtonType) {
			if (gameobject->GetComponent<Button>()->is_clicked) {
				change_entry = gameobject;
				break;
			}
		}
	}

	sf::Event event;
	while (window->pollEvent(event))
	{
		/*ImGui::SFML::ProcessEvent(*window, event);*/
		if (change_entry != nullptr) {
			if (event.type == sf::Event::Closed)
			{
				Engine::GetInstance()->Quit();
				SetSettingsOnFile();
				change_entry->GetComponent<Button>()->is_clicked = false;
				break;
			}

			if (event.type == sf::Event::KeyPressed) {
				std::map<std::string, Input*>* controls = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls();
				for(auto & entry_pair : *controls) {
					Input* entry = controls->at(entry_pair.first);
					if (entry->GetName() == sf::Keyboard::getDescription(event.key.scancode).toAnsiString()) {
						entry->SetName("");
						scene->FindGameObject("button_" + entry_pair.first)->GetComponent<TextRenderer>()->SetString("");
					}
					
				}
				Input* entry_key = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls()->at(change_entry->GetComponent<Button>()->GetTextObject()->GetComponent<TextRenderer>()->GetString());
				entry_key->SetKey(event.key.scancode);
				entry_key->SetName(sf::Keyboard::getDescription(event.key.scancode).toAnsiString());
				change_entry->GetComponent<TextRenderer>()->SetString(sf::Keyboard::getDescription(event.key.scancode).toAnsiString());
				change_entry->GetComponent<Button>()->is_clicked = false;
				break;
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				std::map<std::string, Input*>* controls = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls();
				for (auto& entry_pair : *controls) {
					Input* entry = controls->at(entry_pair.first);
					if (entry->GetName() == "MB" + std::to_string(event.mouseButton.button + 1)) {
						entry->SetName("");
						scene->FindGameObject("button_" + entry_pair.first)->GetComponent<TextRenderer>()->SetString("");
					}
				}
				Input* entry_button = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls()->at(change_entry->GetComponent<Button>()->GetTextObject()->GetComponent<TextRenderer>()->GetString());
				entry_button->SetButton(event.mouseButton.button);
				entry_button->SetName("MB" + std::to_string(event.mouseButton.button + 1));
				change_entry->GetComponent<TextRenderer>()->SetString("MB" + std::to_string(event.mouseButton.button + 1));
				change_entry->GetComponent<Button>()->is_clicked = false;
				break;
			}
		}
		else {

			if (event.type == sf::Event::Closed)
			{
				Engine::GetInstance()->Quit();
				SetSettingsOnFile();
			}
			if (event.type == sf::Event::KeyPressed) {
				pressed[event.key.scancode] = true;
			}
			if (event.type == sf::Event::KeyReleased) {
				pressed[event.key.scancode] = false;
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

					Scene* scene = moduleManager->GetModule<SceneModule>()->GetMainScene();

					for (GameObject* const& button : *scene->GetGameObjects()) {
						if (button->GetType() == ButtonType || button->GetType() == EntryButtonType) {
							if (button->GetComponent<RectangleShapeRenderer>()->GetShape()->getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && !button->GetComponent<Button>()->is_disabled)
							{
								RectangleShapeRenderer* rectangleShapeRenderer = button->GetComponent<RectangleShapeRenderer>();
								SpriteRenderer* spriteRenderer = button->GetComponent<SpriteRenderer>();
								if (button->GetName() == "sound_button" || button->GetName() == "controls_button") {
									button->GetComponent<Button>()->is_clicked = true;
									rectangleShapeRenderer->SetColor(rectangleShapeRenderer->GetClickColor());
									if (spriteRenderer != nullptr) {
										spriteRenderer->SetNextSpriteRect(2);
									}
								}
								else if (button->GetComponent<Button>()->is_clicked) {
									button->GetComponent<Button>()->is_clicked = false;
									rectangleShapeRenderer->SetColor(rectangleShapeRenderer->GetDefaultColor());
									if (spriteRenderer != nullptr) {
										spriteRenderer->SetNextSpriteRect(0);
									}
									if (button->GetName() == "capacity_button") {
										button->GetComponent<Button>()->has_select = nullptr;
										button->GetComponent<Button>()->is_selected_by = nullptr;
										button->GetComponent<Button>()->callback_execute = false;
										scene->FindGameObject("launch_game_button")->GetComponent<Button>()->is_disabled = true;
										if (scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>() != nullptr) {
											scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>()->SetNextSpriteRect(3);
										}
									}
								}
								else {
									button->GetComponent<Button>()->is_clicked = true;
									rectangleShapeRenderer->SetColor(rectangleShapeRenderer->GetClickColor());
									if (spriteRenderer != nullptr) {
										spriteRenderer->SetNextSpriteRect(2);
									}
								}
								if (button->GetName() == "capacity_button") {
									ResetButton(button, { "capacity_button" });
								}
								else if (button->GetName() == "doll_button1" || button->GetName() == "doll_button2" || button->GetName() == "doll_button3") {
									std::vector<std::string> _name = { "doll_button1", "doll_button2", "doll_button3" };
									ResetButton(button, _name);
								}
								else if (button->GetName() == "sound_button" || button->GetName() == "controls_button") {
									std::vector<std::string> _name = { "sound_button", "controls_button" };
									ResetButton(button, _name);
								}
							}
						}
						else if (button->GetType() == SliderButtonType) {
							if (button->GetComponent<RectangleShapeRenderer>()->GetShape()->getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && !button->GetComponent<Button>()->is_disabled) {
								button->GetComponent<Button>()->is_clicked = true;
							}

						}
					}
				}if (event.mouseButton.button == sf::Mouse::Right) {
					mousePos.push_back(sf::Mouse::getPosition(*window));
					float sizeX = (GetWindowSize().x / 100);
					float sizeY = (GetWindowSize().y / 100);
					if (mousePos.size() % 2 == 0) {
						std::cout << mousePos[0].x / sizeX << " " << mousePos[0].y / sizeY << " " << (mousePos[1].x - mousePos[0].x) / sizeX << " " << (mousePos[1].y - mousePos[0].y) / sizeY << std::endl;
						mousePos.clear();
					}
					else {
						std::cout << mousePos[0].x << " " << mousePos[0].y << std::endl;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

					Scene* scene = moduleManager->GetModule<SceneModule>()->GetMainScene();

					for (GameObject* const& button : *scene->GetGameObjects()) {
						if (button->GetType() == SliderButtonType) {
							if (button->GetComponent<RectangleShapeRenderer>()->GetShape()->getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && !button->GetComponent<Button>()->is_disabled) {
								button->GetComponent<Button>()->is_clicked = false;
							}
						}
					}
				}
			}
		}
	}
}

void WindowModule::PreRender()
{
	Module::PreRender();

	window->clear(sf::Color::Black);
}

void WindowModule::Render()
{
	Module::Render();
}

void WindowModule::PostRender()
{
	Module::PostRender();

	window->display();
}

void WindowModule::Release()
{
	Module::Release();

	window->close();
}

void WindowModule::ResetButton(GameObject* button, std::vector<std::string> _name) {
	Scene* scene = moduleManager->GetModule<SceneModule>()->GetMainScene();
	for (GameObject* const& gameObject : *scene->GetGameObjects()) {
		for (std::string button_name : _name) {
			if (gameObject->GetType() == ButtonType && gameObject->GetName() == button_name && button != gameObject && !gameObject->GetComponent<Button>()->is_disabled) {
				gameObject->GetComponent<Button>()->is_clicked = false;
				gameObject->GetComponent<RectangleShapeRenderer>()->SetColor(gameObject->GetComponent<RectangleShapeRenderer>()->GetDefaultColor());
				if (gameObject->GetComponent<SpriteRenderer>() != nullptr) {
					gameObject->GetComponent<SpriteRenderer>()->SetNextSpriteRect(0);
				}
			}
		}
	}
}
