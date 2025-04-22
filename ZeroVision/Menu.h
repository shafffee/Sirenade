#pragma once

#include "Scene.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "GlobalVariables.h"
#include <functional>
#include <algorithm>
#include "SoundPlayer.h"
#include "SpacebarManager.h"

struct MenuElement {
	std::string label;
	std::function<Scene* ()> element;
	std::string audio;
};

class Menu : public Scene {
public:
	Menu() {
	}
	~Menu() {}

	template<typename T>
	void AddMenuOption(const std::string& label, const std::string& audio) {
		this->menu_options.push_back({ label, []() {return new T();}, audio });

		labels.push_back(new sf::Text(*GlobalVariables::font));
		labels[labels.size() - 1]->setFillColor(sf::Color::White);
		labels[labels.size() - 1]->setString(label);
	}

	void onUpdate() override {
		space.update();

		if (first_update) {
			bm.deserialize(GlobalVariables::path_to_beatmaps.string() + "\\" + menu_options[pos].audio + "\\" + menu_options[pos].audio + ".bin");
			sp.setBeatMap(&bm);
			sp.play();
			first_update = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J))
		{
			if (!pressed_next) {
				pressed_next = true;
				if (pos != labels.size() - 1) pos += 1;
				bm.deserialize(	GlobalVariables::path_to_beatmaps.string() + "\\" + menu_options[pos].audio + "\\" + menu_options[pos].audio+".bin");
				sp.setBeatMap(&bm);
				sp.play();
			}
		}
		else pressed_next = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
		{
			if (!pressed_previous) {
				pressed_previous = true;
				if (pos != 0) pos -= 1;
				bm.deserialize(	GlobalVariables::path_to_beatmaps.string() + "\\" + menu_options[pos].audio + "\\" + menu_options[pos].audio+".bin");
				sp.setBeatMap(&bm);
				sp.play();
			}
		}
		else pressed_previous = false;

		if (space.hold())
		{
			if (menu_options[pos].label == "New Game") {
				GlobalVariables::save(0);
			}
			GlobalVariables::scene_manager->attachScene(menu_options[pos].element());
		}
		if (space.tap())
		{
			bm.deserialize(GlobalVariables::path_to_beatmaps.string() + "\\" + menu_options[pos].audio + "\\" + menu_options[pos].audio + ".bin");
			sp.setBeatMap(&bm);
			sp.play();
		}

		if (view_size != GlobalVariables::window->getView().getSize()) {
			view_size = GlobalVariables::window->getView().getSize();
			adaptLabels();
		}

		moveLabels();
		GlobalVariables::window->clear(ClearColor());
		for (sf::Text* label : labels) {
			GlobalVariables::window->draw(*label);
		}
		GlobalVariables::window->display();
	}
private:
	BeatMap bm;
	SoundPlayer sp;

	SpacebarManager space;

	const float font_size = 0.9f;

	bool first_update = true;

	unsigned pos = 0;
	std::vector< MenuElement > menu_options;
	std::vector<sf::Text*> labels;

	float view_offset = 0;

	sf::Vector2f view_size;

	bool pressed_next = false;
	bool pressed_previous = false;

	sf::Color ClearColor() {
		int brightness = (int)(std::min(abs(view_offset-pos),1.0f)*255);
		return sf::Color(brightness,brightness,brightness,255);
	}

	void adaptLabels() {
		for (sf::Text* label : labels) {
			label->setCharacterSize((unsigned)(view_size.y * font_size)); // in pixels, not points!
			if (label->getGlobalBounds().size.x > view_size.x * font_size) {
				//resizing to fit width
				label->setCharacterSize((unsigned)(view_size.y * font_size / label->getGlobalBounds().size.x * view_size.x * font_size));
			}
			label->setOrigin({ (float)(label->getGlobalBounds().size.x / 1.93), (float)(label->getGlobalBounds().size.y / 1.17) });
		}
	}

	void moveLabels() {
		if (view_offset != pos) {
			move_towards(&view_offset, (float)pos);
		}
		
		for (int i = 0;i < labels.size();i++) {
			labels[i]->setPosition({
				view_size.x / 2 + ((float)i-view_offset)* view_size.x,
				view_size.y/2 
				});
		}
	}

	void move_towards(float* a, float b) {

		if (*a == b) return;

		float speed = powf(abs(*a - b),1.2f)*0.2f;
		
		if (speed < 0.000001)*a = b;

		if(*a<b) *a += speed;
		if (*a > b) *a -= speed;
	}
};