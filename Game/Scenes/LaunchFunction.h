#pragma once

class LaunchFunction
{
public:
	template<typename Scene>
	static void LaunchScene();

	template<typename Scene>
	static void LaunchSceneFalse();

	template<typename Scene>
	static void LaunchSceneBackFalse();

	template<typename Scene, typename U>
	static void LaunchSceneParams(std::vector<U*>* tempParams); 
	
	template<typename Scene, typename U>
	static void LaunchSceneFalseParams(std::vector<U*>* tempParams);

	static void LaunchGame(std::vector<Capacity*>* vector_powers);

	static void resumeGame();
};

#include "LaunchFunction.inl"