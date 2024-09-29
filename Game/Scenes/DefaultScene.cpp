#include "DefaultScene.h"
#include "EndLevelScene.h"

#include "LaunchFunction.h"

#include "PauseScene.h"
#include "TransitionScene.h"
#include "DeathScene.h"
#include <iostream>
#include <Interactive.h>
#include "SelectCapacityScene.h"


DefaultScene::DefaultScene(std::vector<Capacity*>* params) : Scene("DefaultScene", [] {})
{
	Engine::SetMusicState(false);
	
	/*if (!music.openFromFile("Game_files/Assets/Sons/musique_level_3.ogg")) {
		std::cout << "La musique ne charge pas" << std::endl;
	}

	music.setLoop(true);
	music.setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
	music.play();*/

	SetTexture("texture_zarya", "Game_files/Assets/Dolls/Zarya-sheet.png");
	SetTexture("texture_zvezda", "Game_files/Assets/Dolls/Zvezda-sheet.png");
	SetTexture("texture_zwezda", "Game_files/Assets/Dolls/Zwezda-sheet.png");
	SetTexture("texture_zarya_gris", "Game_files/Assets/Dolls/texture_zarya_gris.png");
	SetTexture("texture_zvezda_gris", "Game_files/Assets/Dolls/texture_zvezda_gris.png");

	GetBigCapacity()->SetName((*params)[0]->GetName());
	GetMidCapacity()->SetName((*params)[1]->GetName());
	GetSmallCapacity()->SetName((*params)[2]->GetName());


	GetParams()->push_back(GetBigCapacity());
	GetParams()->push_back(GetMidCapacity());
	GetParams()->push_back(GetSmallCapacity());

	SpawnObjectLevel3(params);
	SetTexture("texture_return_button", "Game_files/Assets/button/return_button.png");
	SetTexture("texture_pause_button", "Game_files/Assets/button/pause_button.png");
	GameObject* return_button = CreateSpriteButton_forMainMenu(ButtonType, "button_pause", Maths::Vector2f(window_size.x - window_size.x / 50 - 161, window_size.y / 50), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 161) / 144))), [this] { LaunchFunction::LaunchSceneFalse<PauseScene>(); }, nullptr, "texture_pause_button", Maths::Vector2f(144, 161), Maths::Vector2f(0, 15));

	GameObject* player = CreatePlayer(PlayerType, "Player", Maths::Vector2f(window_size.x / 100 * 6.61458, window_size.y / 100 * 67.1667), Maths::Vector2f((window_size.x / 25), (((window_size.x / 25) * 654) / 420)), "texture_zarya", Maths::Vector2f(420, 654), Maths::Vector2f(0, 1));
	player->GetComponent<Player>()->SetPauseEscape([this] { LaunchFunction::LaunchSceneFalse<PauseScene>(); });
	player->GetComponent<Player>()->SetDeathCallback([this] { LaunchFunction::LaunchSceneFalse<DeathScene>(); });



	float button_size_x = window_size.x / 15 * 3;
	float button_size_y = (((window_size.x / 15) * 3) * 168 / 448);

	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	Scene* transitionScene = scene_module->GetMainScene();
	transitionScene->CreateSpriteButtonWithText(ButtonType, "resume_button", Maths::Vector2f(((window_size.x / 2) - (button_size_x / 2)), window_size.y * 3 / 6), Maths::Vector2f(button_size_x, button_size_y), [transitionScene] {LaunchFunction::resumeGame(); }, nullptr, "texture_button", Maths::Vector2f(448, 168), Maths::Vector2f(0, 24), "RETOUR aU JEU", sf::Color::Black, 30);
	player->GetComponent<Player>()->SetCheckpointCallback([this] {FctForCheckpointMenu(); });
}

void DefaultScene::FctForCheckpointMenu() {
	std::vector<Capacity*>* _params = GetParams();
	LaunchFunction::LaunchSceneFalseParams<SelectCapacityScene>(_params);
}

GameObject* DefaultScene::CreateColliderObject(Scene* scene, const ObjectType& _type, std::string _name, Maths::Vector2f _position, Maths::Vector2f _size) {

	GameObject* game_object = scene->CreateGameObject(_type, _name);
	game_object->SetPosition(_position);

	sf::Vector2f window_size = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize();

	SquareCollider* squareCollider = game_object->CreateComponent<SquareCollider>();
	squareCollider->SetWidth(_size.x);
	squareCollider->SetHeight(_size.y);

	RectangleShapeRenderer* shapeRenderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shapeRenderer->SetColor(sf::Color{ 255,0,0,0 }); // Couleur du mur
	shapeRenderer->SetSize(Maths::Vector2f(_size.x, _size.y)); // Taille du mur

	return game_object;
}

void DefaultScene::SpawnObjectLevel3(std::vector<Capacity*>* params) {
	
	

	sf::Vector2f window_size = sf::Vector2f(Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize().x, Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetWindowSize().y);


	SetTexture("background", "Game_files/Assets/background/Level3.design.png");
	SetBackground("background");
	SetTexture("texture_livre", "Game_files/Assets/Object/livre.png");
	SetTexture("texture_interrupteur", "Game_files/Assets/Object/interrupteur.png");
	SetTexture("texture_fan", "Game_files/Assets/Object/fan_gif.png");
	SetTexture("texture_lampe", "Game_files/Assets/Object/lamp.png");

	SetTexture("texture_placeholder", "Game_files/Assets/button/Placeholder.png");
	SetTexture("texture_dash", "Game_files/Assets/button/dash_button.png");
	SetTexture("texture_double_jump", "Game_files/Assets/button/double_jump_button.png");
	SetTexture("texture_force", "Game_files/Assets/button/force_button.png");
	SetTexture("texture_gravity", "Game_files/Assets/button/gravity_button.png");
	SetTexture("texture_invincibilite", "Game_files/Assets/button/invincibility_button.png");


	float sizeX = window_size.x / 100;
	float sizeY = window_size.y / 100;

	//Sol
	CreateColliderObject(this, ColliderType, "Sol", Maths::Vector2f(sizeX * 0.104167, sizeY * 90.5556), Maths::Vector2f(sizeX * 99.8437, sizeY * 0.462963));

	//Toit
	CreateColliderObject(this, ColliderType, "Toit", Maths::Vector2f(0, 0), Maths::Vector2f(sizeX * 100, sizeY * 0.462963));

	//Etagère + livres
	CreateColliderObject(this, ColliderType, "Etagere", Maths::Vector2f(sizeX * 6.45833, sizeY * 44.9167), Maths::Vector2f(sizeX * 15.8854, sizeY * 1.66667));
	CreateColliderObject(this, ColliderType, "Livre1", Maths::Vector2f(sizeX * 15.7292, sizeY * 34), Maths::Vector2f(sizeX * 1.82292, sizeY * 10.6667));
	CreateColliderObject(this, ColliderType, "Livre2", Maths::Vector2f(sizeX * 17.7083, sizeY * 29.8333), Maths::Vector2f(sizeX * 2.60417, sizeY * 14.75));

	//Table de chevet
	CreateColliderObject(this, ColliderType, "Table_de_chevet", Maths::Vector2f(sizeX * 1.66667, sizeY * 72.8333), Maths::Vector2f(sizeX * 10.3125, sizeY * 20.0833));

	//Ceiling fan
	GameObject* fan = CreateSpriteObject(this, DeathType, "faN", Maths::Vector2f(sizeX * 3.59375, sizeY * 1.57407), Maths::Vector2f(sizeX * 16.875, sizeY * 6.48148), "texture_fan", Maths::Vector2f(523, 185), Maths::Vector2f(0, 1));
	fan->SetIsAnimated(true);
	fan->SetNbAnimationSprite(10);
	fan->SetAnimatedLoop(true);

	//Ours
	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 25.8333, sizeY * 55.1852), Maths::Vector2f(sizeX * 2.70833, sizeY * 5.09259));
	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 32.5, sizeY * 55.0926), Maths::Vector2f(sizeX * 3.17708, sizeY * 5.83333));

	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 27.0833, sizeY * 56.0185), Maths::Vector2f(sizeX * 5.67708, sizeY * 12.8704));

	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 26.875, sizeY * 68.9815), Maths::Vector2f(sizeX * 7.44792, sizeY * 6.66667));
	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 24.8958, sizeY * 73.3333), Maths::Vector2f(sizeX * 11.6146, sizeY * 5.46296));

	CreateColliderObject(this, ColliderType, "Ours", Maths::Vector2f(sizeX * 26.4062, sizeY * 60.1852), Maths::Vector2f(sizeX * 8.38542, sizeY * 8.05556));

	//Fenêtre
	CreateColliderObject(this, ColliderType, "Fenetre", Maths::Vector2f(sizeX * 22.7083, sizeY * 51.6667), Maths::Vector2f(sizeX * 19.7396, sizeY * 1.5));
	
	//Livre mouvable
	CreateSpriteObject(this, MoveType, "LivreMove", Maths::Vector2f(sizeX * 39.7412, sizeY * 40.9), Maths::Vector2f(sizeX * 1.73684, sizeY * 13.7), "texture_livre", Maths::Vector2f(0, 0), Maths::Vector2f(0, 0));

	//Lampe
	GameObject* lampe = CreateOnlySprite(this, GameObjectType, "Lampe", Maths::Vector2f(sizeX * 50.3125, sizeY * 53.1667), Maths::Vector2f(sizeX * 6.875, sizeY * 35.8333), "texture_lampe", Maths::Vector2f(122, 341), Maths::Vector2f(6, 0));

	GameObject* lampe_Bottom = CreateColliderObject(this, DeathType, "Lampe_Bottom", Maths::Vector2f(sizeX * 50.3646, sizeY * 56.75), Maths::Vector2f(sizeX * 6.875, sizeY * 4.62963));
	GameObject* lampe_Top = CreateColliderObject(this, DeathType, "Lampe_Top", Maths::Vector2f(sizeX * 51.3542, sizeY * 53.4167), Maths::Vector2f(sizeX * 4.89583, sizeY * 3.25));


	//Carton
	CreateColliderObject(this, ColliderType, "Carton_Bed", Maths::Vector2f(sizeX * 42.9688, sizeY * 80.9167), Maths::Vector2f(sizeX * 7.34375, sizeY * 12.3333));
	CreateColliderObject(this, ColliderType, "Carton_BedLittle", Maths::Vector2f(sizeX * 50.3125, sizeY * 86.75), Maths::Vector2f(sizeX * 1.5625, sizeY * 6.41667));
	CreateColliderObject(this, ColliderType, "Carton_Desk", Maths::Vector2f(sizeX * 57.0312, sizeY * 63.6667), Maths::Vector2f(sizeX * 5.52083, sizeY * 9.91667));


	//Cadre_du_haut
	CreateColliderObject(this, ColliderType, "Cadre_du_haut", Maths::Vector2f(sizeX * 69.7917, sizeY * 20.5), Maths::Vector2f(sizeX * 11.5104, sizeY * 12.8704));

	//Etagère du bureau
	CreateColliderObject(this, ColliderType, "Etagere_Desk", Maths::Vector2f(sizeX * 61.4583, sizeY * 47.8333), Maths::Vector2f(sizeX * 15.8854, sizeY * 1.66667));
	CreateColliderObject(this, ColliderType, "Livre1_Desk", Maths::Vector2f(sizeX * 66.7188, sizeY * 43.1667), Maths::Vector2f(sizeX * 8.69792, sizeY * 4.66667));
	CreateColliderObject(this, ColliderType, "Livre2_Desk", Maths::Vector2f(sizeX * 69.0625, sizeY * 39.6667), Maths::Vector2f(sizeX * 6.25, sizeY * 3.16667));

	//Bibliotheque
	CreateColliderObject(this, ColliderType, "Bibliotheque", Maths::Vector2f(sizeX * 85.3646, sizeY * 13.1667), Maths::Vector2f(sizeX * 9.84375, sizeY * 75.75));

	//Lit
	CreateColliderObject(this, ColliderType, "lit_pied_gauche", Maths::Vector2f(sizeX * 16.3021, sizeY * 85.2778), Maths::Vector2f(sizeX * 2.29167, sizeY * 8.7963));
	CreateColliderObject(this, ColliderType, "Lit_pied_droite", Maths::Vector2f(sizeX * 42.7083, sizeY * 85.2778), Maths::Vector2f(sizeX * 1.40625, sizeY * 9.25926));
	CreateColliderObject(this, ColliderType, "Lit_matelas", Maths::Vector2f(sizeX * 18.75, sizeY * 79.1667), Maths::Vector2f(sizeX * 23.9583, sizeY * 6.38889));
	CreateColliderObject(this, ColliderType, "Lit_appuis_tete", Maths::Vector2f(sizeX * 18.9062, sizeY * 68.4259), Maths::Vector2f(sizeX * 23.2292, sizeY * 1.11111));

	//Bureau
	CreateColliderObject(this, ColliderType, "Bureau", Maths::Vector2f(sizeX * 62.6562, sizeY * 69.75), Maths::Vector2f(sizeX * 20.2083, sizeY * 1.75));

	//Tabouret
	CreateColliderObject(this, ColliderType, "Tabouret", Maths::Vector2f(sizeX * 72.1354, sizeY * 83.1667), Maths::Vector2f(sizeX * 4.6875, sizeY * 3));
	CreateColliderObject(this, ColliderType, "Tabouret_dossier", Maths::Vector2f(sizeX * 75.4688, sizeY * 76.8333), Maths::Vector2f(sizeX * 1.51042, sizeY * 8.33333));

	//Trou de souris
	GameObject* hole = CreateColliderObject(this, ColliderType, "Trou_de_souris", Maths::Vector2f(sizeX * 96.5104, sizeY * 87.3333), Maths::Vector2f(sizeX * 2.96875, sizeY * 0.25));
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();

	hole->SetCallback([hole, scene_module] { scene_module->SetNextScene([scene_module] { scene_module->SetScene<EndLevelScene>(false); }); });

	if (!hole)
	{
		music.stop();
	}

	//Interrupteur
	GameObject* interrupteur = CreateInteractiveObject(this, InteractiveType, "Interrupteur", Maths::Vector2f(sizeX * 36.5104, sizeY * 61.2037), Maths::Vector2f(sizeX * 1.8958, ((sizeX * 1.8958) * 147) / 94.5), "texture_interrupteur", Maths::Vector2f(189, 294), Maths::Vector2f(0, 13), lampe);
	interrupteur->GetComponent<SpriteRenderer>()->SetNextSpriteRect(1);
	interrupteur->GetComponent<Interactive>()->SetCallback([interrupteur, this] {interrupteur->GetComponent<Interactive>()->SwitchStateLamp(this); });

	//Affichage du pouvoir en cours
	GameObject* Pouvoir_en_cours = CreateOnlySprite(this, GameObjectType, "pouvoir en cours", Maths::Vector2f(window_size.x / 50, window_size.y - window_size.y / 50 - 144), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), "texture_placeholder", Maths::Vector2f(144, 144), Maths::Vector2f(0, 15));

	std::string namePower = (*params)[0]->GetName();
	if (namePower == "INVERSION DE LA GRaVITE")
	{
		Pouvoir_en_cours->GetComponent<SpriteRenderer>()->SetSpriteRect(GetTextureByName("texture_gravity"), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
	}
	else if (namePower == "INVINCIbILITE")
	{
		Pouvoir_en_cours->GetComponent<SpriteRenderer>()->SetSpriteRect(GetTextureByName("texture_invincibilite"), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
	}
	else if (namePower == "DOUbLE SaUT")
	{
		Pouvoir_en_cours->GetComponent<SpriteRenderer>()->SetSpriteRect(GetTextureByName("texture_double_jump"), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
	}
	else if (namePower == "DaSH")
	{
		Pouvoir_en_cours->GetComponent<SpriteRenderer>()->SetSpriteRect(GetTextureByName("texture_dash"), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
	}
	else if (namePower == "fORCE")
	{
		Pouvoir_en_cours->GetComponent<SpriteRenderer>()->SetSpriteRect(GetTextureByName("texture_force"), Maths::Vector2f(window_size.x / 20, ((((window_size.x / 20) * 144) / 144))), Maths::Vector2f(144, 144), Maths::Vector2f(0, 369), Maths::Vector2f(0, 32));
	}
}