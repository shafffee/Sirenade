#pragma once
#include <string>
#include <vector>
#include <fstream>

class Track {
public:
	std::string sound_filename;				//filename of the separate audio layer
	std::vector<float> position_points;		//position on each beat
};

class BeatMap {
public:
	unsigned bpm=100;
	unsigned length = 2; //length in bpm
	int64_t duration = 0;
	std::vector<Track> tracks;
	double  current_b=0;

	BeatMap() {};

	void serialize(const std::string& filename)
	{
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			return;
		}


		//bpm
		file.write(reinterpret_cast<const char*>(&bpm),sizeof(bpm));
		//length in bpm
		file.write(reinterpret_cast<const char*>(&length), sizeof(length));
		//number of tracks
		int number_of_tracks = tracks.size();
		file.write(reinterpret_cast<const char*>(&(number_of_tracks)), sizeof(number_of_tracks));
		//tracks
		for (Track track : tracks) {
			//name len
			int len = track.sound_filename.length();
			file.write(reinterpret_cast<const char*>(&len), sizeof(len));
			//name chars
			for (int i = 0;i < len;i++) {
				char c = track.sound_filename[i];
				file.write(reinterpret_cast<const char*>(&c), sizeof(c));
			}

			//name chars
			for (int i = 0;i < length;i++) {
				float f = track.position_points[i];
				file.write(reinterpret_cast<const char*>(&f), sizeof(f));
			}

		}

		file.close();
	}

	void deserialize(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			return;
		}

		tracks.clear();

		//bpm
		file.read(reinterpret_cast<char*>(&bpm),sizeof(bpm));
		//length in bpm
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		//number of tracks
		int number_of_tracks = 0;
		file.read(reinterpret_cast<char*>(&(number_of_tracks)), sizeof(number_of_tracks));
		//tracks
		for (int t = 0; t < number_of_tracks;t++) {
			tracks.push_back(*(new Track));

			//name len
			int len = 0;
			file.read(reinterpret_cast<char*>(&len), sizeof(len));
			//name chars
			for (int i = 0;i < len;i++) {
				char c = '0';
				file.read(reinterpret_cast<char*>(&c), sizeof(c));
				tracks[t].sound_filename += c;
			}

			//name chars
			for (int i = 0;i < length;i++) {
				float f = 0.0f;
				file.read(reinterpret_cast<char*>(&f), sizeof(f));
				tracks[t].position_points.push_back(f);
			}

		}

		file.close();
	}

	void setBpm(unsigned new_bpm) {
		bpm = new_bpm;
	}

	void setLength(unsigned n_len) {
		if (n_len>length) {
			for (int i = length; i < n_len+2; i++) {
				for (int t = 0; t < tracks.size();t++) {
					tracks[t].position_points.push_back(0);
				}
			}
		}
		if (n_len < length) {
			for (int i = length; i > n_len+2; i--) {
				for (int t = 0; t < tracks.size();t++) {
					tracks[t].position_points.pop_back();
				}
			}
		}
		length = n_len;
	}

	void addTrack(std::string sound_filename) {
		//std::cout << "TRACK ADDED " << sound_filename << std::endl;
		tracks.push_back({ sound_filename, *(new std::vector<float>(length-1,0))});
	}

	void tick(double  delta) {						//every frame
		current_b += delta*bpm/60.0;
	}

	float getTrackPosition(unsigned n) {			//get the point of sound
		if (tracks[n].position_points.size() <= (int)current_b) {
			if (tracks[n].position_points.size() == 0)return 0;
			return tracks[n].position_points[tracks[n].position_points.size()-1];
		}

		return tracks[n].position_points[(int)current_b] + 
			 (tracks[n].position_points[(int)current_b+1]- tracks[n].position_points[(int)current_b])*(current_b-(int)current_b); //connection function
	}

private:
	
};