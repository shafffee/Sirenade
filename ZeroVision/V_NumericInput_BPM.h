#pragma once
#include "Vidget.h"
#include <string>
#include "V_Text.h"

class V_NumericInput_BPM :public Vidget {
public:
	V_Text txt;
	std::string value = "0";

	V_NumericInput_BPM() {
		for (int i = 0;i < 11;i++) {
			pressed[i] = false;
		}
		txt.setColor(sf::Color(0, 125, 125));
	}
	~V_NumericInput_BPM() {}

	void update(BeatMap** beatmap, SoundPlayer* soundplayer) {
		get_input(beatmap, soundplayer);
		update_txt();
	}

private:
	void update_txt() {
		txt.setText(value);

		txt.setPos(get_rel_pos().x, get_rel_pos().y);
		txt.setSize(get_rel_size().x, get_rel_size().y);

		txt.update();
	}
	bool pressed[11];
	void get_input(BeatMap** beatmap, SoundPlayer* soundplayer) {
		bool updated = false;
		for (int i = 0;i < 10;i++) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(int(sf::Keyboard::Key::Num0) + i))) {
				if (!pressed[i]) {
					if (value == "0") value = "";
					value += char(i + '0');
					updated = true;
				}
				pressed[i] = true;
			}
			else {
				pressed[i] = false;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)) {
			if (!pressed[10]) {
				value.pop_back();
				if (value == "") value = "0";
				updated = true;
			}
			pressed[10] = true;
		}
		else {
			pressed[10] = false;
		}

		if (updated) {
			if (atoi(value.c_str()) > 100 && atoi(value.c_str()) < 1000) {
				(*beatmap)->setBpm(atoi(value.c_str()));
				soundplayer->setBeatMap(*beatmap);
			}
		}
	}
};