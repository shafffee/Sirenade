#pragma once
#pragma once

#include "Scene.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "GlobalVariables.h"
#include <functional>
#include <algorithm>

#include "BeatMap.h"
#include "SoundPlayer.h"

#define MARGIN 30.0f

struct GamePartParams {
// 0-text 1-play 2-score
	int type = 0;
	std::string beatmap_name;
};

class GamePart : public Scene {
public:
	GamePartParams params;
	bool ended = false;
	bool request_prev = false;
	   
	GamePart(GamePartParams params)/* : score(*GlobalVariables::font)*/ {
		this->params = params;

		if (params.type != 2) {
			beatmap.deserialize("res\\sound\\" + params.beatmap_name + "\\" + params.beatmap_name + ".bin");
			soundplayer.setBeatMap(&beatmap);
			soundplayer.play();
		}
		else {
			if (GlobalVariables::score > 90) {
				beatmap.deserialize("res\\sound\\grade_S\\grade_S.bin");
			}
			else if (GlobalVariables::score > 80) {
				beatmap.deserialize("res\\sound\\grade_A\\grade_A.bin");
			}
			else if (GlobalVariables::score > 70) {
				beatmap.deserialize("res\\sound\\grade_B\\grade_B.bin");
			}
			else if (GlobalVariables::score > 60) {
				beatmap.deserialize("res\\sound\\grade_C\\grade_C.bin");
			}
			else if (GlobalVariables::score > 50) {
				beatmap.deserialize("res\\sound\\grade_D\\grade_D.bin");
			}
			else {
				beatmap.deserialize("res\\sound\\grade_F\\grade_F.bin");
				request_prev = true;
			}
			soundplayer.setBeatMap(&beatmap);
			soundplayer.play();
		}

		//score.setPosition({ 0,0 });
		//score.setCharacterSize(50);
	}
	~GamePart() {}

	void onUpdate() {
		soundplayer.update();
		space.update();

		if (soundplayer.isPlaying() == false) {
			if (params.type != 2) {
				ended = true;
			}
			if (params.type == 1) {
				GlobalVariables::score = 100.0 * gained / total;
			}
		}

		draw();
		GlobalVariables::window->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			soundplayer.stop();
			GlobalVariables::menu_request = true;
			return;
		}

		if (space.hold()) {
			if (params.type==0) {
				soundplayer.stop();
				ended = true;
			}
			if (params.type==1) {
				gained = 0;
				total = 0;
				soundplayer.stop();
				soundplayer.play();
			}
			if (params.type == 2) {
				soundplayer.stop();
				ended = true;
			}
		}

		if (space.tap()) {
			if (params.type == 2) {
				soundplayer.stop();
				soundplayer.play();
			}
		}
	}
private:
	BeatMap beatmap;
	SoundPlayer soundplayer;
	SpacebarManager space;

	//sf::CircleShape c;
	//sf::Text score;

	double gained = 0;
	double total = 0;
	double punishment =1;

	double error = 0;
	double error_speed = 0.01    ;

	void draw() {
		if (params.type==1) {     
			/*
			score.setString(std::to_string(100.0*gained/total));

			c.setRadius(50);
			c.setOrigin({ 50,50 });
			*/
			float value = 0;
			for (int i = 0; i < soundplayer.beatmap->tracks.size();i++) {
				value += soundplayer.beatmap->getTrackPosition(i);
			}
			value /= soundplayer.beatmap->tracks.size();
			
			//scoring
			bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
			bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);
			if (value >= 0.99) {
				total += 1;
				if (right && !left) {
					gained += 1;
					if (error - error_speed > 0) {
						error -= error_speed;
					}
				}
				else {
					gained -= punishment;
					if (error + error_speed < 1.0) {
						error += error_speed;
					}
				}
			}
			else if (value > 0.01) {
				total += 1;
				if (!left) {
					gained += 1;
					if (error - error_speed > 0) {
						error -= error_speed;
					}
				}
				else {
					gained -= punishment;
					if (error + error_speed < 1.0) {
						error += error_speed;
					}
				}
			}
			else if (abs(value) <0.01 ) {
				total += 1;
				if (!right && !left) {
					gained += 1;
					if (error - error_speed > 0) {
						error -= error_speed;
					}
				}
				else {
					gained -= punishment;
					if (error + error_speed < 1.0) {
						error += error_speed;
					}
				}
			}
			else if (value > -0.99) {
				total += 1;
				if (!right) {
					gained += 1;
					if (error - error_speed > 0) {
						error -= error_speed;
					}
				}
				else {
					gained -= punishment;
					if (error + error_speed < 1.0) {
						error += error_speed;
					}
				}
			}
			else {
				total += 1;
				if (!right && left) {     
					gained += 1;
					if (error - error_speed > 0) {
						error -= error_speed;
					}
				}
				else {
					gained -= punishment;
					if (error + error_speed < 1.0) {
						error += error_speed;
					}
				}
			}

			/*
			if (abs(value)==1) {
				c.setFillColor(sf::Color(0, 255, 255));
			}
			else {
				c.setFillColor(sf::Color(255, 255, 255));
			}
			
			c.setPosition({ GlobalVariables::window->getView().getSize().x / 2+value* GlobalVariables::window->getView().getSize().x / 2, GlobalVariables::window->getView().getSize().y / 2 });
			GlobalVariables::window->draw(c);
			GlobalVariables::window->draw(score);
			*/
		}
		GlobalVariables::window->clear(sf::Color(int(error * 255.0), 0, 0));
	}
};