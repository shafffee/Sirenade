#pragma once
#include "Vidget.h"
#include "SoundPlayer.h"
#include "V_FileManager.h"

class V_Editor:public Vidget {
private:
	bool play_pressed = true;
	bool save_pressed = true;
	bool split_increase_pressed = true;
	bool split_decrease_pressed = true;
public:

	int64_t start_point = 0;
	unsigned split = 2;

	V_Editor() {}
	~V_Editor() {}

	void update(BeatMap* beatmap, SoundPlayer* soundplayer, V_FileManager* fm){
		sf::VertexArray horizontal_separation(sf::PrimitiveType::Lines, 2 * (beatmap->tracks.size() + 1));
		float h_sep;
		if (beatmap->tracks.size() == 0) {
			h_sep = get_size().y;
		}
		else {
			h_sep = (get_size().y) / beatmap->tracks.size();
		}

		sf::VertexArray split_draw(sf::PrimitiveType::Lines, 2 * (beatmap->tracks.size()*split + 1));
		for (int i = 0; i < beatmap->tracks.size()*split + 1;i++) {
			split_draw[i * 2].position = sf::Vector2f(get_pos().x, get_pos().y + i * h_sep/split);
			split_draw[i * 2 + 1].position = sf::Vector2f(get_pos().x + get_size().x, get_pos().y + i * h_sep/split);
			split_draw[i * 2].color = sf::Color(100, 0, 60);
			split_draw[i * 2 + 1].color = sf::Color(100, 0, 60);
		}
		GlobalVariables::window->draw(split_draw);

		for (int i = 0; i < beatmap->tracks.size() + 1;i++) {
			horizontal_separation[i * 2].position = sf::Vector2f(get_pos().x, get_pos().y + i * h_sep);
			horizontal_separation[i * 2 + 1].position = sf::Vector2f(get_pos().x + get_size().x, get_pos().y + i * h_sep);
		}
		GlobalVariables::window->draw(horizontal_separation);

		sf::VertexArray vertical_separation(sf::PrimitiveType::Lines, 2 * (beatmap->length + 1));
		float v_sep;
		if (beatmap->length == 0) {
			v_sep = get_size().x;
		}
		else {
			v_sep = get_size().x / beatmap->length;
		}
		for (int i = 0; i < beatmap->length + 1;i++) {
			vertical_separation[i * 2].position = sf::Vector2f(get_pos().x+i * v_sep, get_pos().y);
			vertical_separation[i * 2 + 1].position = sf::Vector2f(get_pos().x + i * v_sep, get_pos().y+get_size().y);

			vertical_separation[i * 2].color = sf::Color(50, 50, 50);
			vertical_separation[i * 2 + 1].color = sf::Color(50, 50, 50);
		}
		GlobalVariables::window->draw(vertical_separation);


		std::vector<sf::SoundBuffer> b;
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color(0,125,125,125));
		for (int i = 0; i < soundplayer->sounds.size();i++) {
			b.push_back(soundplayer->sounds[i]->getBuffer());
		}
		for (int i = 0; i < b.size();i++) {
			const int step = 250;
			double size_h = get_size().x / b[i].getSampleCount();
			//double size_h = 1;
			double size_v_multi = 1.0f * h_sep / std::numeric_limits<uint16_t>::max();
			//float size_v_multi = 1.0f;
			for (int j = 0; j < b[i].getSampleCount();j+=step) {
				//std::cout << b[i].getSamples()[j]<<"\t"<< float(1.0 * abs(b[i].getSamples()[j]) * size_v_multi * 2) << std::endl;
				rect.setSize({ float(size_h*step), float(1.0*abs(b[i].getSamples()[j]) * size_v_multi*2) });
				rect.setOrigin({ float(0), float(1.0*abs(b[i].getSamples()[j]) * size_v_multi) });
				rect.setPosition({ float(1.0*j* size_h+get_pos().x),float( 1.0f * get_pos().y + h_sep * 0.5f+i*h_sep)});
				GlobalVariables::window->draw(rect);
			}
		}

		sf::CircleShape point;
		point.setRadius(3);
		point.setOrigin({ 3, 3 });
		point.setFillColor(sf::Color(0, 25, 255));

		//progress
		sf::VertexArray progress(sf::PrimitiveType::Lines, 2);
		progress[0].position = sf::Vector2f(get_size().x * beatmap->current_b / beatmap->length + get_pos().x, get_pos().y);
		progress[1].position = sf::Vector2f(get_size().x * beatmap->current_b / beatmap->length + get_pos().x, get_pos().y+get_size().y);
		progress[0].color = sf::Color(255, 0, 0);
		progress[1].color = sf::Color(255, 0, 0);
		GlobalVariables::window->draw(progress);

		for (int y = 0; y < beatmap->tracks.size();y++) {
			sf::VertexArray path(sf::PrimitiveType::LineStrip, beatmap->tracks[y].position_points.size());
			for (int x = 0; x < beatmap->tracks[y].position_points.size();++x) {
				point.setPosition({
					x * v_sep + get_pos().x,
					0.5f * h_sep + y * h_sep + beatmap->tracks[y].position_points[x] * 0.5f * h_sep + get_pos().y
					});
				path[x].position = point.getPosition();
				path[x].color = sf::Color(0, 255, 255);
				GlobalVariables::window->draw(point);
			}
			GlobalVariables::window->draw(path);
		}

		//star
		sf::VertexArray start(sf::PrimitiveType::Lines, 2);
		start[0].position = sf::Vector2f(get_size().x * start_point / beatmap->duration / (1.0 * beatmap->length / beatmap->bpm * 60) * (beatmap->duration * 0.000001) + get_pos().x, get_pos().y);
		start[1].position = sf::Vector2f(get_size().x * start_point / beatmap->duration / (1.0 * beatmap->length / beatmap->bpm * 60) * (beatmap->duration * 0.000001) + get_pos().x, get_pos().y + get_size().y);
		start[0].color = sf::Color(0, 255, 0);
		start[1].color = sf::Color(0, 255, 0);
		GlobalVariables::window->draw(start);

		//CONTROLS
		
		sf::Vector2i mousepos = sf::Mouse::getPosition(*GlobalVariables::window);
		if (mousepos.x > get_pos().x && mousepos.x < get_pos().x + get_size().x &&
			mousepos.y>get_pos().y && mousepos.y < get_pos().y + get_size().y
			) {

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				int y = (mousepos.y - get_pos().y) / h_sep;
				int x = (mousepos.x + v_sep / 2 - get_pos().x) / v_sep;
				float value = (mousepos.y - y * h_sep - h_sep / 2 - get_pos().y) / h_sep * 2;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
					beatmap->tracks[y].position_points[x] = value;
				}
				else {
					value = round(value * split * 0.5) / split / 0.5;
					beatmap->tracks[y].position_points[x] = value;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
					for (int i = 0; i < beatmap->tracks.size();i++) {
						beatmap->tracks[i].position_points[x] = value;
					}
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				int64_t value = beatmap->duration * (mousepos.x - get_pos().x) / get_size().x * (1.0 * beatmap->length / beatmap->bpm * 60) / (beatmap->duration * 0.000001);
				if (value < beatmap->duration) {
					start_point = beatmap->duration * (mousepos.x - get_pos().x) / get_size().x * (1.0 * beatmap->length / beatmap->bpm * 60) / (beatmap->duration * 0.000001);
					if (!soundplayer->isPlaying()) {
						soundplayer->go(sf::microseconds(start_point));
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			{
				if (!play_pressed) {
					if (soundplayer->isPlaying()) {
						soundplayer->stop();
						soundplayer->go(sf::microseconds(start_point));
					}
					else {
						soundplayer->go(sf::microseconds(start_point));
						soundplayer->play();
					}
				}
				play_pressed = true;
			}
			else {
				play_pressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				if (!save_pressed) {
					beatmap->serialize(fm->getBeatmapPath());
				}
				save_pressed = true;
			}
			else {
				save_pressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				if (!split_increase_pressed) {
					if(split<16)split += 2;
				}
				split_increase_pressed = true;
			}
			else {
				split_increase_pressed = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				if (!split_decrease_pressed) {
					if(split>2)split -= 2;
				}
				split_decrease_pressed = true;
			}
			else {
				split_decrease_pressed = false;
			}

		}
		
	}

};