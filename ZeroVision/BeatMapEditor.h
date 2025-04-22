#pragma once
#pragma once

#include "Scene.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "GlobalVariables.h"
#include <functional>
#include <iostream>
#include <algorithm>

#include "BeatMap.h"
#include "SoundPlayer.h"
#include "V_Editor.h"
#include "V_FileManager.h"
#include "V_NumericInput_BPM.h"

#define MARGIN 30.0f

class BeatMapEditor : public Scene {
public:
	BeatMap* beatmap;

	BeatMapEditor() {
		this->setBeatMap(GlobalVariables::beatmap);
		editor.setSize(0.9,0.7);
		editor.setPos(0.05,0.25);

		fm.setSize(0.9, 0.23);
		fm.setPos(0.05, 0.01);

		ni.setSize(0.1, 0.1);
		ni.setPos(0.9, 0.0);
	}
	~BeatMapEditor() {}

	void setBeatMap(BeatMap* beatmap) {
		this->beatmap = beatmap;
		soundplayer.setBeatMap(beatmap);
	}

	void onUpdate() {
		soundplayer.update();

		GlobalVariables::window->clear();
		draw();
		GlobalVariables::window->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			soundplayer.stop();
			GlobalVariables::menu_request = true;
			return;
		}
	}
private:
	SoundPlayer soundplayer;
	V_Editor editor;
	V_FileManager fm;
	V_NumericInput_BPM ni;

	void draw() {
		fm.update(&beatmap, &soundplayer, &ni);
		editor.update(beatmap, &soundplayer, &fm);
		ni.update(&beatmap, &soundplayer);
	}
};