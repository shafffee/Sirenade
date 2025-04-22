#pragma once
#include "SceneManager.h"
#include "BeatMap.h"
#include "Menu.h"

// res\sound\beatmap_name\beatmap_name.bin

namespace GlobalVariables {
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ 960,540 }), "SFML works!", sf::Style::None, sf::State::Fullscreen);
	sf::Font* font = new sf::Font("res\\fonts\\LeagueGothic\\LeagueGothic-Regular-VariableFont_wdth.ttf");
	extern SceneManager* scene_manager = new SceneManager;
	BeatMap* beatmap = nullptr;
	std::filesystem::path path_to_beatmaps("res\\sound");

	bool menu_request = false;

	double score = 0;

	std::filesystem::path savefile("res\\save\\save.bin");

	void save(int n) {
		std::ofstream file(savefile, std::ios::binary);
		if (!file.is_open()) {
			return;
		}
		file.write(reinterpret_cast<const char*>(&n), sizeof(n));
		file.write(reinterpret_cast<const char*>(&score), sizeof(score));
		file.close();
	};
	int load() {
		std::ifstream file(savefile, std::ios::binary);
		if (!file.is_open()) {
			return 0;
		}
		int n = 0;
		double s = 0;
		//bpm
		file.read(reinterpret_cast<char*>(&n), sizeof(n));
		file.read(reinterpret_cast<char*>(&s), sizeof(s));
		score = s;
		return n;
	};
}