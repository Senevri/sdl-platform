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


class CGob{

public:
	typedef struct {
		SDL_Surface * icon;
		std::string name;
		SDL_Rect * loc;
		SDL_Rect * move;
		bool visible;
	} gob;

	//constructor, destructor
	CGob();
	~CGob();

	//methods
	void create(std::string name);
	void setIcon(CGob::gob * g, SDL_Surface *bitmap);
	void setIcon(std::string name, SDL_Surface *bitmap);
	void draw(SDL_Surface * screen, std::string name);
	void load(std::string bitmap, std::string name);
	void move(std::string, int, int);
	void move(std::string);
	CGob::gob * find(std::string name);
	void hide(std::string name){ gob * g = this->find(name); g->visible=false; }
	void show(std::string name){ gob * g = this->find(name); g->visible=true; }

private:
	std::vector<gob> gobs;
};
