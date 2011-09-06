#include <iostream>
#include <cmath>
#include "Zounds.h"

using namespace std;	

Zounds::Zounds(){
	m_rate = 44100;
	m_buffer = 4096;
	m_channels = 2;
	m_format = AUDIO_S16SYS; // AUDIO_U8 and AUDIO_S16SYS work, at least
	m_songs = NULL; m_sound = NULL;
	if(Mix_OpenAudio(m_rate, m_format, m_channels, m_buffer) != 0) { 
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError()); 
		exit(1); 
	}
	Mix_QuerySpec(&m_rate, &m_format, &m_channels);
}
Zounds::~Zounds(){
	Mix_FreeChunk(m_sound);
	Mix_CloseAudio();
}

void cbtest() {
	//Zounds::musicDone();
}

void Zounds::playMusic(std::string filename, int loops){
//	int pos = filename.find(".");
//	std::string typestring = filename.substr(pos);
	if (m_songs == NULL) {
		// Linux problem workaround
		cout << "playMusic:" << filename  << endl;
		if (filename.length() > filename.find(".ogg")) {
			Mix_SetMusicCMD("mplayer");
		} else if  (filename.length() > filename.find(".mod")) {
			cout << "using mikmod" <<endl;
			Mix_SetMusicCMD("mikmod -q");
		}

		m_songs = Mix_LoadMUS(filename.c_str());
		cout << m_songs <<endl;
		Mix_PlayMusic(m_songs, loops);
		Mix_HookMusicFinished((void (*)())&Zounds::musicDone);
	//	Mix_HookMusicFinished(cbtest);
	} else {
		musicDone();
		m_songs = NULL;
		playMusic(filename, loops);
	}
}


int Zounds::playSound(Mix_Chunk * chunk, int loops){
	return Mix_PlayChannel(-1, chunk, loops);
}

int Zounds::playSound(std::string filename, int loops){
	if (NULL == m_sound) {
		m_sound = Mix_LoadWAV(filename.c_str());
		return Mix_PlayChannel(-1, m_sound, loops);
	} else {
		return Mix_PlayChannel(-1, m_sound, loops);		
	} 
}

void Zounds::musicDone(){
	cout << "musicDone" <<endl;
	Mix_HaltMusic();
	Mix_FreeMusic(m_songs);
}

Mix_Chunk * Zounds::genSine(int duration, double hz, bool hibits) {
	const double PI = 3.141592653589793;
	int val = 0;
	int amplitude = (hibits ? 16384 : 128 );
	Mix_Chunk * sound = static_cast<Mix_Chunk *>(malloc(sizeof(Mix_Chunk)));
	sound->allocated = 1;
	sound->volume=127;
	sound->alen = m_rate * duration * (hibits ? 2 : 1);
//	cout << "duration:" << sound->alen << endl;
	sound->abuf = static_cast<Uint8 *>(calloc(sound->alen, sizeof(Uint8)));
	for (long i = 0; i < sound->alen; i=i+(hibits ? 2 : 1)) {
		val =  amplitude * sin((PI*i*hz)/m_rate);
		if (hibits) {
			char * ca = (char *) &val;
			sound->abuf[i] = ca[0];
			sound->abuf[i+1] = ca[1];
		} else {
			sound->abuf[i] = val + (m_format == AUDIO_U8 ? 127 : 0);	
		}
	}
	return sound;
}

