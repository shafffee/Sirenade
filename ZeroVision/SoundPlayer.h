#pragma once

#include <SFML/Audio.hpp>
#include "BeatMap.h"


class SoundPlayer {
public:

	void setBeatMap(BeatMap* new_beatmap) {
		beatmap = nullptr;
		stop();
		this->beatmap = new_beatmap;
		buffers.clear();
		sounds.clear();
		is_playing = false;
		for (Track track : beatmap->tracks) {
			buffers.push_back(new sf::SoundBuffer());
			if (!buffers[buffers.size() - 1]->loadFromFile(track.sound_filename)) {
				//error while loading sound
			};
			beatmap->duration = std::max(beatmap->duration, buffers[buffers.size() - 1]->getDuration().asMicroseconds());
			beatmap->setLength( (unsigned)(1.0 * buffers[buffers.size() - 1]->getDuration().asSeconds() / 60 * beatmap->bpm) + 1);


			sounds.push_back(new sf::Sound(*(buffers[buffers.size() - 1])));

			for (sf::Sound* sound : sounds) {
				sound->setRelativeToListener(true);
			}  
		}
	}
	void update() {
		deltaTime = deltaClock.getElapsedTime();
		deltaClock.restart();

		if (is_playing) {
			beatmap->tick(deltaTime.asSeconds());
			//set relative to listener
			for (int i = 0; i < sounds.size();i++) {
				sounds[i]->setPosition(getSoundPos(beatmap->getTrackPosition(i)));
			}
		}

		for (sf::Sound* sound : sounds) {
			if(sound->getStatus() == sf::SoundSource::Status::Stopped){
				stop();
			}
		}

	};
	void play() {
		is_playing = true;
		for (sf::Sound* sound : sounds) {
			sound->play();
		}
	}
	void stop() {
		is_playing = false;
		for (sf::Sound* sound : sounds) {
			sound->stop();
		}
		if (beatmap != nullptr) {
			beatmap->current_b = 0;
		}
	}
	void pause() {
		is_playing = false;
		for (sf::Sound* sound : sounds) {
			sound->pause();
		}
	}
	void go(sf::Time time) {
		for (sf::Sound* sound : sounds) {
			sound->setPlayingOffset(time);
		}
		beatmap->current_b = beatmap->bpm*time.asMicroseconds()*0.000001/60;
	}
	bool isPlaying() {
		return is_playing;
	}

	std::vector<sf::Sound*> sounds;
	BeatMap* beatmap;
private:
	sf::Clock deltaClock;
	sf::Time deltaTime;


	bool is_playing = false;
	std::vector<sf::SoundBuffer*> buffers;

	const float dist = 1.3f;
	sf::Vector3f getSoundPos(float track_position) {
		if (abs(track_position) > 0.99 || abs(track_position) < 0.01) {
			return { sin(track_position) , cos(track_position) ,0 };
		}
		else {
			return { sin(track_position)*dist , cos(track_position)*dist ,0 };
		}
	}
};