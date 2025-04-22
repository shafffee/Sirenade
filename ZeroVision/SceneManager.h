#pragma once

#include "Scene.h"

class SceneManager {
public:
	SceneManager(){
	};
	~SceneManager() {};

	void update() {
		current_scene->onUpdate();
	};
	void attachScene(Scene* current_scene_pointer) {
		delete(current_scene);
		current_scene = current_scene_pointer;
	};
private:
	Scene* current_scene = nullptr;
	sf::RenderWindow* window = nullptr;
};