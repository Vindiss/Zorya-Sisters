#include "Components/TextRenderer.h"
#include "Engine.h"
#include "Modules/SceneModule.h"
#include <iostream>

TextRenderer::TextRenderer()
{
	text = new sf::Text();
}

TextRenderer::~TextRenderer()
{
	delete text;
	text = nullptr;
}

void TextRenderer::Render(sf::RenderWindow* _window)
{
	ARendererComponent::Render(_window);

	GameObject* owner = GetOwner();

	const Maths::Vector2<float> position = owner->GetPosition();
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
	text->setString(string);
	text->setFont(*scene->GetFont());
	text->setCharacterSize(caractere_size);
	if (text_name != "") {
		text->setPosition(special_position.x, special_position.y);
	}
	else {
		text->setPosition(position.x+(GetSize().x/2)-text->getGlobalBounds().width/2, position.y + (GetSize().y / 2) - text->getGlobalBounds().height);
	}
	
	text->setFillColor(color);
	_window->draw(*text);
}