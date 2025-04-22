#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalVariables.h"

class Vidget {
private:
	float size[2] = { 1.0f,1.0f };
	float pos[2] = { 0.0f,0.0f };
public:
	Vidget() {}
	virtual ~Vidget() {}


	void setSize(float x, float y) {
		size[0] = x;
		size[1] = y;
	}
	void setPos(float x, float y) {
		pos[0] = x;
		pos[1] = y;
	}

	sf::Vector2f get_size() {
		return sf::Vector2f(
			GlobalVariables::window->getView().getSize().x * size[0],
			GlobalVariables::window->getView().getSize().y * size[1]
		);
	};

	sf::Vector2f get_rel_size() {
		return sf::Vector2f(
			size[0],
			size[1]
		);
	};

	sf::Vector2f get_pos() {
		return sf::Vector2f(
			GlobalVariables::window->getView().getSize().x * pos[0],
			GlobalVariables::window->getView().getSize().y * pos[1]
		);
	};

	sf::Vector2f get_rel_pos() {
		return sf::Vector2f(
			pos[0],
			pos[1]
		);
	};
};