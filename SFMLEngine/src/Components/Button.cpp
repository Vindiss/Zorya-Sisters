#include "Components/Button.h"
#include <iostream>
#include <Engine.h>
#include "Modules/SceneModule.h"
#include "Modules/InputModule.h"
#include <fstream>
#include <sstream>


void Button::Execute() {
	if (GetOwner()->GetType() == SliderButtonType) {
		if (is_clicked) {
			callback();
		}
	}
	else {
		if (!callback_execute && is_clicked) {
			callback();
			callback_execute = true;
		}
		if (!is_clicked) {
			callback_execute = false;
		}
	}
}


void Button::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	CheckHover();
	Execute();
}

void Button::CheckHover() {
	sf::Vector2i mousePos = sf::Mouse::getPosition(*Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow());
	RectangleShapeRenderer* rectangleShapeRenderer = GetOwner()->GetComponent<RectangleShapeRenderer>();
	SpriteRenderer* spriteRenderer = GetOwner()->GetComponent<SpriteRenderer>();

	if (!is_clicked && !is_disabled) {
		if (rectangleShapeRenderer->GetShape()->getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
			rectangleShapeRenderer->SetColor(rectangleShapeRenderer->GetHoverColor());
			if (spriteRenderer != nullptr) {
				spriteRenderer->SetNextSpriteRect(1);
			}
		}
		else {
			rectangleShapeRenderer->SetColor(rectangleShapeRenderer->GetDefaultColor());
			if (spriteRenderer != nullptr) {
				spriteRenderer->SetNextSpriteRect(0);
			}
		}
	}
}


void Button::DollSelectCapacity() {
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	bool have_capacity_button = false;
	for (GameObject* const& gameObject : *scene->GetGameObjects()) {
		if (gameObject->GetName() == "capacity_button") {
			have_capacity_button = true;
			break;
		}
	}
	if (!have_capacity_button) {
		sf::Vector2u window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow()->getSize();

		std::ifstream file("Game_files/capacity.csv");

		if (!file.is_open()) {
			std::cerr << "Error opening filebb." << std::endl;
		}

		int nb_ligne = 0;
		int num_capacity = 1;
		std::string line;

		while (std::getline(file, line)) {
			nb_ligne++;
		}

		file.clear();
		file.seekg(0, std::ios::beg);

		while (std::getline(file, line)) {
			std::vector<std::string> row;
			std::stringstream lineStream(line);
			std::string cell;

			while (std::getline(lineStream, cell, ',')) {
				row.push_back(cell);
			}

			Capacity* capacity = new Capacity();
			capacity->SetName(row[0]);
			capacity->SetDescription(row[1]);
			scene->SetTexture("texture_capacity_button_" + std::to_string(num_capacity), "Game_files/Assets/button/" + row[2]);
			GameObject* button = scene->CreateSpriteButton_forMainMenu(ButtonType, "capacity_button", Maths::Vector2f((window_size.x / 2) - (window_size.x / 15 / 2 * (nb_ligne % 2)) - (window_size.x / 15 * (nb_ligne / 2)) - (((window_size.x / 15) / 2) * (nb_ligne - 1) / 2) + (window_size.x / 15 * (num_capacity - 1)) + ((window_size.x / 15) / 2 * (num_capacity - 1)), (window_size.y / 100 * 10)),
				Maths::Vector2f(window_size.x / 15, window_size.x / 15), [] {}, capacity, "texture_capacity_button_" + std::to_string(num_capacity), Maths::Vector2f(144,161), Maths::Vector2f(0,15));
			button->GetComponent<Button>()->SetCallback(std::bind(&Button::SelectCapacity, button->GetComponent<Button>()));
			num_capacity++;
		}

		file.close();

		GameObject* text_for_capacity_name = scene->CreateText(TextType, "text_for_capacity_name", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 25)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 10)), 40);
		GameObject* text_for_capacity_description = scene->CreateText(TextType, "text_for_capacity_description", Maths::Vector2f(window_size.x / 2 - window_size.x / 4, (window_size.y / 100 * 40)), sf::Color::White, Maths::Vector2u(window_size.x / 4 * 2, (window_size.y / 100 * 5)), 30);
	}
	else {
		for (GameObject* const& gameObject : *scene->GetGameObjects()) {
			if (gameObject->GetName() == "capacity_button") {
				if(gameObject->GetComponent<Button>()->is_selected_by != GetOwner() && gameObject->GetComponent<Button>()->is_selected_by != nullptr){
					gameObject->GetComponent<RectangleShapeRenderer>()->SetColor(gameObject->GetComponent<RectangleShapeRenderer>()->GetDisabledColor());
					gameObject->GetComponent<Button>()->is_disabled = true;
					if (gameObject->GetComponent<SpriteRenderer>() != nullptr) {
						gameObject->GetComponent<SpriteRenderer>()->SetNextSpriteRect(3);
					}
				}
				else if(gameObject->GetComponent<Button>()->is_selected_by == GetOwner()){
					gameObject->GetComponent<RectangleShapeRenderer>()->SetColor(gameObject->GetComponent<RectangleShapeRenderer>()->GetClickColor());
					gameObject->GetComponent<Button>()->is_disabled = false;
					if (gameObject->GetComponent<SpriteRenderer>() != nullptr) {
						gameObject->GetComponent<SpriteRenderer>()->SetNextSpriteRect(2);
					}
				}
				else {
					gameObject->GetComponent<RectangleShapeRenderer>()->SetColor(gameObject->GetComponent<RectangleShapeRenderer>()->GetDefaultColor());
					gameObject->GetComponent<Button>()->is_disabled = false;
					if (gameObject->GetComponent<SpriteRenderer>() != nullptr) {
						gameObject->GetComponent<SpriteRenderer>()->SetNextSpriteRect(0);
					}
				}
			}
		}
	}
}

void Button::SelectCapacity() {
	if (is_selected_by == nullptr) {
		Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
		scene->FindGameObject("text_for_capacity_name")->GetComponent<TextRenderer>()->SetString(GetObject()->GetName());
		scene->FindGameObject("text_for_capacity_description")->GetComponent<TextRenderer>()->SetString(GetObject()->GetDescription());
		int capacity_selected = 0;
		for (GameObject* const& gameObject : *scene->GetGameObjects()) {
			if ((gameObject->GetName() == "doll_button1" || gameObject->GetName() == "doll_button2" || gameObject->GetName() == "doll_button3")){
				if (gameObject->GetComponent<Button>()->is_clicked) {
					if (gameObject->GetComponent<Button>()->has_select != nullptr) {
						gameObject->GetComponent<Button>()->has_select->GetComponent<Button>()->is_selected_by = nullptr;
					}
					gameObject->GetComponent<Button>()->has_select = GetOwner();
					is_selected_by = gameObject;
				}
				if (gameObject->GetComponent<Button>()->has_select != nullptr) {
					capacity_selected++;
				}
			}
		}
		if (capacity_selected == 3) {
			scene->FindGameObject("launch_game_button")->GetComponent<Button>()->is_disabled = false;
			if (scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>() != nullptr) {
				scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>()->SetNextSpriteRect(0);
			}
		}
		else {
			scene->FindGameObject("launch_game_button")->GetComponent<Button>()->is_disabled = true;
			if (scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>() != nullptr) {
				scene->FindGameObject("launch_game_button")->GetComponent<SpriteRenderer>()->SetNextSpriteRect(3);
			}
		}
	}
}

void Button::SoundMenu() {
	SetSettingsOnFile();
	sf::Vector2u window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow()->getSize();
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	auto controls = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls();
	for (const auto& entry : *controls) {
		scene->DestroyGameObject(scene->FindGameObject("text_" + entry.first));
		scene->DestroyGameObject(scene->FindGameObject("button_" + entry.first));
	}

	GameObject* sound_text = scene->CreateText(TextType, "sound_text", Maths::Vector2f(window_size.x / 100 * 28.6979, window_size.y / 2 - (((window_size.x / 100 * 38.2812) * 49) / 500)), sf::Color::Black, Maths::Vector2u(window_size.x / 100 * 13.4375, (((window_size.x / 100 * 38.2812) * 49) / 500)), 40);
	sound_text->GetComponent<TextRenderer>()->SetString("VolumE");
	GameObject* jauge_slider = scene->CreateOnlySprite(scene, GameObjectType, "jauge_slider", Maths::Vector2f(window_size.x / 100 * 42, window_size.y / 2 - (((window_size.x / 100 * 38.2812) * 49) / 500)), Maths::Vector2f(window_size.x / 100 * 38.2812, (((window_size.x / 100 * 38.2812) * 49) / 500)), "texture_jauge_slider", Maths::Vector2f(500, 49), Maths::Vector2f(0, 0));
	GameObject* sliderButton = scene->CreateSliderButton(SliderButtonType, "sound_slider_button", Maths::Vector2f(jauge_slider->GetPosition().x + 4, window_size.y / 2 - (((window_size.x / 100 * 38.2812) * 49) / 500) + ((((window_size.x / 100 * 38.2812) * 49) / 500) * 0.5) - (window_size.y / 100 * 6.27778 * 0.5)), Maths::Vector2f((((window_size.y / 100 * 6.27778) * 41) / 40), window_size.y / 100 * 6.27778), [] {}, nullptr, "texture_button_slider", Maths::Vector2f(41, 40), Maths::Vector2f(0, 8));
	
	float size_x_slider = (jauge_slider->GetPosition().x + jauge_slider->GetComponent<SpriteRenderer>()->GetWidth() - (sliderButton->GetComponent<SpriteRenderer>()->GetWidth()) - 4 - jauge_slider->GetPosition().x + 4);
	float pos_for_volume = size_x_slider / 100 * Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume();
	sliderButton->SetPosition(Maths::Vector2f(sliderButton->GetPosition().x + pos_for_volume, sliderButton->GetPosition().y));
	
	sliderButton->GetComponent<Button>()->SetCallback(std::bind(&Button::SliderMove, sliderButton->GetComponent<Button>()));
	GameObject* volume_text = scene->CreateText(TextType, "volume_text", Maths::Vector2f(sliderButton->GetPosition().x, sliderButton->GetPosition().y + (window_size.y / 100 * 6)), sf::Color::Black, Maths::Vector2u((((window_size.y / 100 * 6.27778) * 41) / 40), window_size.y / 100 * 3), 20);
	volume_text->GetComponent<TextRenderer>()->SetString(std::to_string(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume()));
}

void Button::ControlsMenu() {
	SetSettingsOnFile();
	sf::Vector2u window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindow()->getSize();
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	scene->DestroyGameObject(scene->FindGameObject("sound_slider_button"));
	scene->DestroyGameObject(scene->FindGameObject("sound_text"));
	scene->DestroyGameObject(scene->FindGameObject("jauge_slider"));
	scene->DestroyGameObject(scene->FindGameObject("volume_text"));
	auto controls = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls();
	int nb_entry = 0;

	for (const auto& entry : *controls) {
		nb_entry++;
		GameObject* entry_text = scene->CreateText(TextType, "text_" + entry.first, Maths::Vector2f(window_size.x / 100 * 40.5208, (window_size.y / 100 * 22.2222) + (nb_entry * window_size.y / 100 * 3.87037) + (nb_entry * window_size.y / 100 * 5.47037)), sf::Color::Black, Maths::Vector2u(window_size.x / 100 * 22.8646, window_size.y / 100 * 3.87037), 45);
		entry_text->GetComponent<TextRenderer>()->SetString(entry.first);
		GameObject* bouton_entry = scene->CreateSpriteButtonWithText(EntryButtonType, "button_" + entry.first, Maths::Vector2f(window_size.x / 100 * 74.5208, (window_size.y / 100 * 22.2222) + (nb_entry * window_size.y / 100 * 3.87037) + (nb_entry * window_size.y / 100 * 5.47037) - window_size.y / 100 * 3.87037 / 2), Maths::Vector2f((window_size.y / 100 * 3.87037 * 2) * 448 / 168, window_size.y / 100 * 3.87037 * 2), [] { }, nullptr, "texture_entry_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), entry.second->GetName(), sf::Color::Black, 45);
		bouton_entry->GetComponent<Button>()->SetCallback(std::bind(&Button::ChangeEntry, bouton_entry->GetComponent<Button>()));
		bouton_entry->GetComponent<Button>()->SetTextObject(entry_text);
		entry.second->GetEntry();
	}
}

void Button::SliderMove() {
	WindowModule* windowModule = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();
	sf::Vector2i mousePos = windowModule->GetMousePosition();
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();

	if (GetOwner()->GetComponent<RectangleShapeRenderer>()->GetShape()->getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
		GameObject* jauge_slider = scene->FindGameObject("jauge_slider");
		if (jauge_slider->GetPosition().x + 4 + (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth() / 2) < mousePos.x && jauge_slider->GetPosition().x + jauge_slider->GetComponent<SpriteRenderer>()->GetWidth() - 4 - (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth() / 2) > mousePos.x) {
			float size_one_vol = (jauge_slider->GetPosition().x + jauge_slider->GetComponent<SpriteRenderer>()->GetWidth() - (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth()) - 4 - jauge_slider->GetPosition().x + 4) / 100;
			GetOwner()->SetPosition(Maths::Vector2f(mousePos.x - (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth() / 2), GetOwner()->GetPosition().y));
			Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->SetSoundVolume((GetOwner()->GetPosition().x - jauge_slider->GetPosition().x + 4) / size_one_vol);
			scene->FindGameObject("volume_text")->GetComponent<TextRenderer>()->SetString(std::to_string(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume()));
			scene->FindGameObject("volume_text")->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().x, scene->FindGameObject("volume_text")->GetPosition().y));
		}
		else if (jauge_slider->GetPosition().x + 4 + (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth() / 2) >= mousePos.x) {
			GetOwner()->SetPosition(Maths::Vector2f(jauge_slider->GetPosition().x + 4, GetOwner()->GetPosition().y));
			Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->SetSoundVolume(0);
			scene->FindGameObject("volume_text")->GetComponent<TextRenderer>()->SetString(std::to_string(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume()));
			scene->FindGameObject("volume_text")->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().x, scene->FindGameObject("volume_text")->GetPosition().y));
		}
		else {
			GetOwner()->SetPosition(Maths::Vector2f(jauge_slider->GetPosition().x + jauge_slider->GetComponent<SpriteRenderer>()->GetWidth() - (GetOwner()->GetComponent<SpriteRenderer>()->GetWidth()) - 4, GetOwner()->GetPosition().y));
			Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->SetSoundVolume(100);
			scene->FindGameObject("volume_text")->GetComponent<TextRenderer>()->SetString(std::to_string(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume()));
			scene->FindGameObject("volume_text")->SetPosition(Maths::Vector2f(GetOwner()->GetPosition().x, scene->FindGameObject("volume_text")->GetPosition().y));
		}
	}
	else {
		is_clicked = false;
	}
}

void Button::ChangeEntry() {
	GetOwner()->GetComponent<TextRenderer>()->SetString("");
	Input* entry_key = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls()->at(GetOwner()->GetComponent<Button>()->GetTextObject()->GetComponent<TextRenderer>()->GetString());
	entry_key->SetName("");
}