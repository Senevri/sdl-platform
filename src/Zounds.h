#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

class Zounds{
	public:
		Zounds();
		~Zounds();
		int playSound(Mix_Chunk * chunk, int loops=0);
		Mix_Chunk * genSine(int duration, double hz, bool hibits = false);
		void playMusic(std::string filename, int loops=0);
		int playSound(std::string filename, int loops=0);
	private:
		Mix_Chunk * m_sound;
		int m_rate;
		Uint16 m_format;
		int m_channels;
		int m_buffer;
		Mix_Music * m_songs;
		void musicDone();

};
