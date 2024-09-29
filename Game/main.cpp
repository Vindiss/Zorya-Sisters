#include "Engine.h"
#include "SceneModule.h"
#include "Scenes/IntroScene.h"


int main()
{
	const Engine* engine = Engine::GetInstance();

	engine->Init();

	SceneModule* scene_module = engine->GetModuleManager()->GetModule<SceneModule>();
	scene_module->SetScene<IntroScene>();
	engine->Run();


	return 0;
}
