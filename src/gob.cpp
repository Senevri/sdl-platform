#include "gob.h"
/**
 * Constructor, Destructor
 */

CGob::CGob(){

}


CGob::~CGob(){
	for(Uint32 i=0;i<gobs.size();i++){
		delete gobs[i].loc;
		delete gobs[i].move;
	}
	gobs.clear();
}

void CGob::create(std::string name){
	gob g;
	g.name = name;	
	g.loc = new SDL_Rect();
	g.loc->x=0;
	g.loc->y=0;
	g.move = new SDL_Rect();
	g.move->x=0;
	g.move->y=0;
	g.visible=true;
	this->gobs.push_back(g);
}

void CGob::setIcon(std::string name, SDL_Surface *bitmap){
	gob *g=find(name);
	g->icon = bitmap;
}

/* Way faster */
void CGob::setIcon(CGob::gob * g, SDL_Surface *bitmap){
	g->icon = bitmap;
}

void CGob::load(std::string bitmap, std::string name){
	gob g;
	g.name = name;
	g.icon = SDL_LoadBMP(bitmap.c_str());
	g.loc = new SDL_Rect();
	g.loc->x=0;
	g.loc->y=0;
	g.move = new SDL_Rect();
	g.move->x=0;
	g.move->y=0;
	this->gobs.push_back(g);
}

CGob::gob * CGob::find(std::string name){
	gob *g = NULL;
	for(Uint8 i=0;i<gobs.size();i++){
		if(0==(gobs[i].name.compare(name))){
			g = &gobs[i];
		}
	}
	return g;
}

void CGob::draw(SDL_Surface *screen, std::string name){
	gob *g=find(name);
	SDL_SetClipRect(screen, NULL);
	SDL_BlitSurface(g->icon, NULL, screen, g->loc);
}

void CGob::move(std::string name, int x, int y){
	gob *g=find(name);
	int newx;
	newx = g->loc->x + x;
	int newy;
	newy = g->loc->y + y;
}

void CGob::move(std::string name){
	gob *g=find(name);
	int newx;
	newx = g->loc->x + g->move->x;
	int newy;
	newy = g->loc->y + g->move->y;
	if (newx<0||newy<0) {
		return;
	}
	g->loc->x = newx;
	g->loc->y = newy;
}

