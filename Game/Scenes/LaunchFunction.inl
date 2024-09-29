#pragma once

#include "DefaultScene.h"
#include "TransitionScene.h"

template<typename Scene>
void LaunchFunction::LaunchScene() {
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module] { scene_module->SetScene<Scene>(); });
}

template<typename Scene>
void LaunchFunction::LaunchSceneFalse() {
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module] { scene_module->SetScene<Scene>(false); });
}

template<typename Scene>
void LaunchFunction::LaunchSceneBackFalse() {
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module] { scene_module->SetSceneBack<Scene>(false); });
}


//inline void LaunchFunction::LaunchGame() {
//	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();
//	std::vector<Capacity> params;
//	params.push_back(*scene->FindGameObject("doll_button1")->GetComponent<Button>()->has_select->GetComponent<Button>()->GetObject());
//	params.push_back(*scene->FindGameObject("doll_button2")->GetComponent<Button>()->has_select->GetComponent<Button>()->GetObject());
//	params.push_back(*scene->FindGameObject("doll_button3")->GetComponent<Button>()->has_select->GetComponent<Button>()->GetObject());
//	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
//	scene_module->SetNextScene([scene_module, params] { scene_module->SetSceneWithParamsBack<DefaultScene>(false, params); });
//}

inline void LaunchFunction::LaunchGame(std::vector<Capacity*>* vector_powers) {
	Scene* scene = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>()->GetMainScene();	

	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module, vector_powers] { scene_module->SetSceneWithParamsBack<DefaultScene>(false, vector_powers); });
}


template<typename Scene, typename U>
void LaunchFunction::LaunchSceneParams(std::vector<U*>* tempParams) { 
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module, tempParams] { scene_module->SetSceneWithParams<Scene>(true, tempParams); });
}

template<typename Scene, typename U>
void LaunchFunction::LaunchSceneFalseParams(std::vector<U*>* Params) {
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetNextScene([scene_module, Params] { scene_module->SetSceneWithParams<Scene>(false, Params); });
}


inline void LaunchFunction::resumeGame() {
	SceneModule* scene_module = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
	Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>()->GetPressed()->clear();

	std::vector<Scene*>* scenes = scene_module->GetScenesVector();
	scene_module->SetMainScene((*scenes)[scenes->size() - 2]);
	scenes->erase(scenes->end() - 1);
}