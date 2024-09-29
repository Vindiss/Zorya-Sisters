#include "Components/Interactive.h"
#include "Engine.h"
#include "Modules/InputModule.h"

void Interactive::Update(const float _delta_time, std::unordered_map<sf::Keyboard::Scancode, bool>* pressed_input) {
	if (can_be_activated) {
		TextRenderer* text_renderer = GetOwner()->CreateComponent<TextRenderer>();
		text_renderer->SetTextName("interagir");
		text_renderer->SetSpecialPosition(Maths::Vector2f(GetOwner()->GetPosition().x + GetOwner()->GetComponent<SquareCollider>()->GetWidth() + 10, GetOwner()->GetPosition().y));
		text_renderer->SetColor(sf::Color::Black);
		text_renderer->SetSize(Maths::Vector2f(120, 20));
		text_renderer->SetCaracterSize(20);
		text_renderer->SetString("E pour interagir");
		InputModule* input_module = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>();
		if (input_module->GetControls()->at("IntERagiR")->EntryIsPressesd()) {
			if (can_activate_clock.getElapsedTime().asSeconds() >= 0.1) {
				callback();
				can_activate_clock.restart();
			}
		}
		else {
			can_activate_clock.restart();
		}
	}
	else {
		TextRenderer* text_renderer = GetOwner()->GetComponent<TextRenderer>();
		if (text_renderer != nullptr) {
			GetOwner()->RemoveComponent(text_renderer);
		}
	}
}

void Interactive::SwitchStateLamp(Scene* scene) {
	if (is_activated) {
		GetOwner()->GetComponent<SpriteRenderer>()->SetNextSpriteRect(1);
		object->GetComponent<SpriteRenderer>()->SetNextSpriteRect(0);
		is_activated = false;
	}
	else {
		GetOwner()->GetComponent<SpriteRenderer>()->SetNextSpriteRect(0);
		object->GetComponent<SpriteRenderer>()->SetNextSpriteRect(1);
		is_activated = true;
		lampe_Bottom = scene->FindGameObject("Lampe_Bottom");
		lampe_Top = scene->FindGameObject("Lampe_Top");
		lampe_Bottom->SetType(ColliderType);
		lampe_Top->SetType(ColliderType);
	}
}