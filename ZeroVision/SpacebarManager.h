#pragma once
#include <SFML/Window.hpp>

class SpacebarManager {
private:
	sf::Clock deltaClock;
	sf::Time deltaTime;

	BeatMap b;
	SoundPlayer sp;

	float duration = 0;
	bool h = false;
	bool t = false;

	bool just_started = true;

public:

	SpacebarManager() {
		b.deserialize(GlobalVariables::path_to_beatmaps.string() + "\\press\\press.bin");
		sp.setBeatMap(&b);
	}

	void update() {
		deltaTime = deltaClock.getElapsedTime();
		deltaClock.restart();
		if (!just_started) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
				if (duration == 0) {
					sp.play();
				}
				duration += deltaTime.asMilliseconds();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || duration >= 600) {
				if (duration != 0) {
					if (duration <= 300) {
						t = true;
					}
					if (duration >= 600) {
						sp.play();
						h = true;
					}
					duration = 0;
				}
			}
		}
		else {
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
				just_started = false;
			}
		}
	}

	bool tap() {
		bool res = t;
		t = false;
		return res;
	}

	bool hold() {
		bool res = h;
		h = false;
		return res;
	}
};