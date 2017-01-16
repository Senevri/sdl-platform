#include "gob.h"
/**
 * Constructor, Destructor
 */

Gob::Gob(){

}


Gob::~Gob(){
	for(Uint32 i=0;i<gobs.size();i++){
		try {
			//SDL_FreeSurface(gobs[i].icon);
			delete gobs[i].loc;
			delete gobs[i].move;
		} catch (std::exception ex) {
			std::printf(ex.what());
		}
	}
	gobs.clear();
}

void Gob::create(std::string name){
	gob g;
	g.texture = nullptr;	
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

void Gob::setIcon(std::string name, SDL_Surface *bitmap){
	gob *g=find(name);
	setIcon(g, bitmap);
}

/* Way faster */
void Gob::setIcon(Gob::gob * g, SDL_Surface *bitmap){
	g->icon = bitmap;
	g->loc = &bitmap->clip_rect;
}

Gob::gob * Gob::load(std::string bitmap, std::string name){
	gob g;
	g.texture = nullptr;
	g.name = name;
	//leak?
	g.icon = SDL_LoadBMP(bitmap.c_str());
	g.loc = new SDL_Rect();
	g.loc->x=0;
	g.loc->y=0;
	g.loc->w = g.icon->w;
	g.loc->h = g.icon->h;
	g.move = new SDL_Rect();
	g.move->x=0;
	g.move->y=0;
	this->gobs.push_back(g);
	return find(name);
}

Gob::gob * Gob::find(std::string name){
	gob *g = NULL;
	for(Uint8 i=0;i<gobs.size();i++){
		if(0==(gobs[i].name.compare(name))){
			g = &gobs[i];
			break;
		}
	}
	return g;
}

void Gob::draw(SDL_Renderer *screen, std::string name){
	gob *g=find(name);
	if (nullptr == g) return;

	if (g->texture == nullptr) { // FIXME not nullpointer because I don't know!
		g->texture = SDL_CreateTextureFromSurface(screen, g->icon);
		//SDL_FreeSurface(g->icon);
	} else {
		//
	}

	SDL_RenderCopy(screen, g->texture, NULL, g->loc);			
}

void Gob::move(std::string name, int x, int y){
	gob *g=find(name);
	int newx;
	newx = g->loc->x + x;
	int newy;
	newy = g->loc->y + y;
}

void Gob::move(std::string name){
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

