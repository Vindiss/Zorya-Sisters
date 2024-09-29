#include "Engine.h"

#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Modules/SceneModule.h"


#include <iostream>


Engine* Engine::instance = nullptr;
sf::Music Engine::music;

Engine* Engine::GetInstance()
{
	if (instance == nullptr)
		instance = new Engine();

	return instance;
}

void Engine::Init() const
{
	moduleManager->CreateDefaultModules();
	moduleManager->Init();



	if (!music.openFromFile("Game_files/Assets/Sons/musique_level_3.ogg")) {
		std::cout << "La musique ne charge pas" << std::endl;
	}
	music.setLoop(true);
	music.setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
}
 
void Engine::Run() const 
{
	moduleManager->Start();

	while (!shouldQuit)
	{
		moduleManager->Update();
		moduleManager->PreRender();
		moduleManager->Render();
		moduleManager->PostRender();
		if (moduleManager->GetModule<SceneModule>()->GetNextScene()) {
			moduleManager->GetModule<SceneModule>()->GetNextScene()();
			moduleManager->GetModule<SceneModule>()->SetNextScene([] {});
		}
	}

	moduleManager->Release();
	moduleManager->Finalize();
}


void Engine::SetMusicState(bool _state) {
	if (_state && music.getStatus() != sf::Sound::Playing)
	{
		music.stop();
		music.play();
	}
	else if (!_state && music.getStatus() == sf::SoundSource::Playing)
	{
		//Faire une descente progressive du son ?
		music.pause();
		//au lieu de mettre ne pause on peut mettre une autre musique maybe ? / Faire une fonction à appeler dans TransitionScene ou Default
		music.setVolume(Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetSoundVolume());
	}
	
}

void SetSettingsOnFile() {

	const std::string nomFichier = "Game_files/settings.csv";
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();

	std::ofstream fichierCSV(nomFichier);

	if (!fichierCSV.is_open()) {
		std::cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << std::endl;
	}

	fichierCSV << "Volume," << std::to_string(scene_module->GetSoundVolume()) << "," << std::endl;
	fichierCSV << ",," << std::endl;

	std::map<std::string, Input*>* controls = Engine::GetInstance()->GetModuleManager()->GetModule<InputModule>()->GetControls();
	for (auto& entry_pair : *controls) {
		Input* entry = controls->at(entry_pair.first);
		fichierCSV << entry_pair.first << "," << entry->GetName() << "," << std::to_string(entry->GetEntry()) << std::endl;
	}

	fichierCSV.close();
}
