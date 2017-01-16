#pragma once 

#include "SDL.h"
#include <vector>
#include <string>
/**
 *	Graphical Objects
 */
/**
 *	Done all wrong. Rant.
 *	This is more like a Gobhandler, with gobs themselves being simple structs. This is going to bite me in 
 *	the ass, soon.
 */


class Gob{

public:
	typedef struct {
		SDL_Surface * icon;
		SDL_Texture * texture;
		std::string name;		
		SDL_Rect * loc;
		SDL_Rect * move;
		bool visible;
	} gob;

	//constructor, destructor
	Gob();
	~Gob();

	//methods
	void create(std::string name);
	void setIcon(Gob::gob * g, SDL_Surface *bitmap);
	void setIcon(std::string name, SDL_Surface *bitmap);
	void draw(SDL_Renderer * renderer, std::string name);
	Gob::gob * load(std::string bitmap, std::string name);
	void move(std::string, int, int);
	void move(std::string);
	Gob::gob * find(std::string name);
	void hide(std::string name){ gob * g = this->find(name); g->visible=false; }
	void show(std::string name){ gob * g = this->find(name); g->visible=true; }

private:
	std::vector<gob> gobs;
};
