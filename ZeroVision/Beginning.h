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

class Beginning : public Scene {
public:
	SceneManager manager;
	int current = 0;
	std::vector<GamePartParams> parts = {
		//playable	filename
		{ 3, "intro" },
		{ 0, "menu_tutorial" },
	};
	GamePart* scene;

	Beginning() {
		scene = new GamePart(parts[current]);
		manager.attachScene(scene);
	}
	~Beginning() {}

	void onUpdate() {
		if (scene->ended) {
			current += 1;
			if (current >= parts.size()) {
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
