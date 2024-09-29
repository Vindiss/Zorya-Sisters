#pragma once
#include <vector>

#include "Module.h"
#include "Scene.h"
#include "TimeModule.h"
#include "WindowModule.h"
#include <functional>

class SceneModule final : public Module
{
public:
	SceneModule();
	~SceneModule() = default;

	void Start() override;
	void Render() override;
	void Update() override;

	void SetNextScene(std::function<void()> _next_scene) { nextScene = _next_scene; }
	void SetSoundVolume(int _sound_volume) { sound_volume = _sound_volume; }

	template<typename T>
	Scene* SetScene(bool _replace_scenes = true);
	void SetMainScene(Scene* _scene) { mainScene = _scene; };
	template<typename T>
	Scene* SetSceneBack(bool _replace_scenes = true);

	template<typename T, typename U>
	Scene* SetSceneWithParams(const bool _replace_scenes, std::vector<U*>* params);
	template<typename T, typename U>
	Scene* SetSceneWithParamsBack(const bool _replace_scenes, std::vector<U*>* params);

	Scene* GetMainScene() const { return mainScene; }
	Scene* GetScene(const std::string& _scene_name) const;
	std::function<void()> GetNextScene() { return nextScene; }
	std::vector<Scene*>* GetScenesVector() { return &scenes; }

	sf::Font GetFont() { return font; }
	int GetSoundVolume() { return sound_volume; }

private:
	std::vector<Scene*> scenes;
	Scene* mainScene = nullptr;
	std::function<void()> nextScene;

	WindowModule* windowModule = nullptr;
	TimeModule* timeModule = nullptr;

	sf::Font font;
	int sound_volume = 0;
};

template<typename T>
Scene* SceneModule::SetSceneBack(const bool _replace_scenes)
{
	if (_replace_scenes)
	{
		for (const Scene* scene : scenes)
		{
			delete scene;
		}
		scenes.clear();
	}

	Scene* scene = static_cast<Scene*>(new T());
	Scene* temp = scenes[scenes.size() - 1];

	scenes[scenes.size() - 1] = scene;
	scenes.push_back(temp);

	return scene;
}

template<typename T, typename U>
Scene* SceneModule::SetSceneWithParamsBack(const bool _replace_scenes, std::vector<U*>* params)
{
	if (_replace_scenes)
	{
		for (const Scene* scene : scenes)
		{
			delete scene;
		}
		scenes.clear();
	}

	Scene* scene = static_cast<Scene*>(new T(params));
	Scene* temp = scenes[scenes.size() - 1];

	scenes[scenes.size() - 1] = scene;
	scenes.push_back(temp);

	return scene;
}

template<typename T>
Scene* SceneModule::SetScene(const bool _replace_scenes)
{
	if (_replace_scenes)
	{
		for (const Scene* scene : scenes)
		{
			delete scene;
		}
		scenes.clear();
	}

	Scene* scene = static_cast<Scene*>(new T());
	scenes.push_back(scene);

	//if (_replace_scenes)
	mainScene = scene;

	return scene;
}

template<typename T, typename U>
Scene* SceneModule::SetSceneWithParams(const bool _replace_scenes, std::vector<U*>* params)
{
	if (_replace_scenes)
	{
		for (const Scene* scene : scenes)
		{
			delete scene;
		}
		scenes.clear();
	}

	Scene* scene = static_cast<Scene*>(new T(params));
	scenes.push_back(scene);

	//if (_replace_scenes)
	mainScene = scene;

	return scene;
}