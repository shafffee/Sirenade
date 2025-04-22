#pragma once
#pragma once
#include "Vidget.h"
#include "V_Text.h"
#include "V_NumericInput_BPM.h"

#include <filesystem>
namespace fs = std::filesystem;

class V_FileManager :public Vidget {
private:

	std::vector<std::string> dirs;
	unsigned opened_dir = -1;
	std::vector<V_Text> music_files;

	bool next_pressed = false;
	bool prev_pressed = false;


	//V_Text hint;
	V_Text projectname;
public:

	int64_t start_point = 0;
	std::string beatmap_path = "";

	V_FileManager() {
		projectname.setColor(sf::Color(0, 125, 125));
		//hint.setColor(sf::Color(125, 125, 0));
		update_directories();
	}

	~V_FileManager() {}

	void update(BeatMap** beatmap, SoundPlayer* soundplayer, V_NumericInput_BPM* ni) {

		if (opened_dir == -1) {
			opened_dir = 0;
			update_directories();
			open(beatmap, soundplayer, ni);
			projectname.setText(dirs[opened_dir]);
		}

		align_projectname(&projectname);
		//align_hint(&hint);
		//hint.update();
		projectname.update();

		align_filenames(&music_files);
		for (int i = 0;i < music_files.size();i++) {
			music_files[i].update();
		}

		//CONTROLS
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			if (!prev_pressed) {
				update_directories();
				if (opened_dir > 0) {
					opened_dir -= 1;
					open(beatmap, soundplayer, ni);
					projectname.setText(dirs[opened_dir]);
				}
			}
			prev_pressed = true;
		}
		else {
			prev_pressed = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			if (!next_pressed) {
				update_directories();
				if (opened_dir < dirs.size() - 1) {
					opened_dir += 1;
					open(beatmap, soundplayer, ni);
					projectname.setText(dirs[opened_dir]);
				}
			}
			next_pressed = true;
		}
		else {
			next_pressed = false;
		}

	}

	void update_directories() {
		dirs.clear();
		for (auto& entry : fs::directory_iterator(GlobalVariables::path_to_beatmaps)) {
			if (fs::is_directory(entry.path())) {
				dirs.push_back(fs::relative(entry.path()).string());
			}
		}
	}
	void open(BeatMap** beatmap, SoundPlayer* soundplayer, V_NumericInput_BPM* ni) {
		music_files.clear();

		bool bm_found = false;
		for (auto& entry : fs::directory_iterator(dirs[opened_dir])) {
			if (entry.path().extension() == ".bin") {
				beatmap_path = fs::relative(entry.path()).string();
				(*beatmap)->deserialize(beatmap_path);
				bm_found = true;
			}
		}
		if (!bm_found) {
			//delete(beatmap);
			beatmap_path = fs::relative(dirs[opened_dir]).string()+"\\"+fs::relative(dirs[opened_dir]).filename().string() + ".bin";
			(*beatmap) = new BeatMap();
		}

		for (auto& entry : fs::directory_iterator(dirs[opened_dir])) {
			if (entry.path().extension() == ".wav") {
				bool found = false;
				for (Track t : (*beatmap)->tracks) {
					if (fs::relative(t.sound_filename).filename() == entry.path().filename()) {
						found = true;
						break;
					}
				}
				if (!found) {
					(*beatmap)->addTrack(fs::relative(entry.path()).string());
				}
				music_files.push_back(V_Text(fs::relative(entry.path()).string()));
			}
		}

		soundplayer->setBeatMap(*beatmap);
		ni->value = std::to_string((*beatmap)->bpm);
		//GlobalVariables::beatmap = 
	};

	std::string getBeatmapPath() {
		return beatmap_path;
	}

	/*
	void align_hint(V_Text* vtxt) {
		vtxt->setSize(get_rel_size().x, get_rel_size().y * 0.15f);
		vtxt->setPos(get_rel_pos().x, get_rel_pos().y + get_rel_size().y - vtxt->get_rel_size().y);
	}
	*/

	void align_filenames(std::vector<V_Text>* vtxt) {
		float els = get_rel_size().y * 0.7f / vtxt->size();
		for (int i = 0;i < vtxt->size();i++) {
			(*vtxt)[i].setSize(get_rel_size().x, els);
			(*vtxt)[i].setPos(get_rel_pos().x, get_rel_pos().y + get_rel_size().y*0.3+i*els);
		}
	}

	void align_projectname(V_Text* vtxt) {
		vtxt->setSize(get_rel_size().x, get_rel_size().y * 0.3f);
		vtxt->setPos(get_rel_pos().x, get_rel_pos().y);
	}

};