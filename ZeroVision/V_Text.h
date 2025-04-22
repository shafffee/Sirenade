#pragma once
#include "Vidget.h"

class V_Text :public Vidget {
private:
	sf::Text text;
	const float font_size_relative = 0.90f;
public:

	int64_t start_point = 0;

	V_Text(sf::String str="text") : text(sf::Text(*GlobalVariables::font)) {
		text.setFillColor(sf::Color::White);
		setText(str);
	}
	~V_Text() {}

	void update() {
		adapttext();
	}

	void setText(sf::String str) {
		text.setString(str);
	}

	void setColor(sf::Color c) {
		text.setFillColor(c);
	}

	void adapttext() {
			text.setCharacterSize((unsigned)(get_size().y * font_size_relative)); // in pixels, not points!
			if (text.getGlobalBounds().size.x > get_size().x * font_size_relative) {
				//resizing to fit width
				text.setCharacterSize((unsigned)(get_size().y * font_size_relative / text.getGlobalBounds().size.x * get_size().x * font_size_relative));
			}
			text.setPosition({
				get_size().x*(1.0f-font_size_relative) / 2 + get_pos().x,
				get_size().y* (1.0f - font_size_relative) / 2 + get_pos().y
				});
			GlobalVariables::window->draw(text);
	}

};