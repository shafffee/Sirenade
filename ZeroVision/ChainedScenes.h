#pragma once

#include "Scene.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "GlobalVariables.h"
#include <functional>
#include <algorithm>

#include "BeatMap.h"
#include "SoundPlayer.h"
#include "GamePart.h"
#include "Menu.h"

#define MARGIN 30.0f

class ChainedScenes : public Scene {
public:
	SceneManager manager;
	int current = 0;
	std::vector<GamePartParams> parts = {
		//playable	filename
		{ 0, "chapter0" },
		{ 0, "beatmap_tutorial" },
		{ 1, "Track1" },
		{2, "RESULT"},
		{ 0, "chapter1" },
		{ 1, "Track2" },
		{2, "RESULT"},
		{ 0, "chapter2" },
		{ 1, "Track3" },
		{2, "RESULT"},
		{ 0, "chapter3" },
		{ 1, "Track4" },
		{2, "RESULT"},
		{ 0, "chapter4" },
	};
	GamePart* scene;

	ChainedScenes() {
		current = GlobalVariables::load();
		scene =new GamePart(parts[current]);
		manager.attachScene(scene);
	}
	~ChainedScenes() {}

	void onUpdate() {
		if (scene->ended) {
			if (!scene->request_prev) {
				current += 1;
				if (!(scene->params.type == 1 && GlobalVariables::score <= 50)) {	//if didn't lose
					GlobalVariables::save(current);
				}
			}
			else {
				current -= 1;
				GlobalVariables::save(current); 
			}
			if (current >= parts.size()) {
				GlobalVariables::save(0);
				GlobalVariables::menu_request = true;
				return;
			}
			scene = new GamePart(parts[current]);
			manager.attachScene(scene);
		}
		manager.update();
	}
private:
};