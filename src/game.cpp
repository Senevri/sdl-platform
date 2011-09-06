#include "game.h"
#include <iostream>
#include <cmath>
#include "InputHandler.h"
#include "Console.h"
#include "Zounds.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BITS 32

using namespace std;
//Main loop necessary to run...

int main(int argc, char *argv[]) {    
	CMyGame * p = new CMyGame();
	p->runGame();
	delete p;	
	return 0;
}

//constructor, destructor

/***********************************************************************
*Desc: The game encapsulated as a class.
*
*By: Esa Karjalainen, 04. June, 2005
***********************************************************************/
CMyGame::CMyGame(){
	testx = 0;
	testy = 0;	
	initialize();
}

//destructor
CMyGame::~CMyGame(){	
	// clear all data
	// Shutdown all subsystems
	delete znd;
	delete gobs;
	TTF_Quit();
	SDL_Quit();	
	exit(0);
}

// ***methods start

//public
/***********************************************************************
*Desc: method/func for starting the game...
*
*By: Esa Karjalainen, 04. June, 2005
***********************************************************************/
void CMyGame::runGame(){
	// put some stuff on screen
	//testScreen();
	//testIcon();
	testGob();
	testZounds();
	//the one, the only....
	mainLoop();
}


//private
/***********************************************************************
*Desc: game object's initializer routine.
*
*By: Esa Karjalainen, 04. June, 2005
***********************************************************************/
bool CMyGame::testGobMove(int action){
	enum actions {JUMP, LEFT, RIGHT, CONSOLE, QUIT};
	CGob::gob * g = this->gobs->find("test");
	int max_speed = 6;
	if (action == LEFT) {
		if (g->move->x>-max_speed && g->loc->y>(SCREEN_HEIGHT-65)) {
			testx += -1;
		}
	}
	if (action == RIGHT) {
		if (g->move->x<max_speed && g->loc->y>(SCREEN_HEIGHT-65)) {
			testx += 1;
		}
	}
	if (action == JUMP) {
		if (g->move->y>-max_speed && g->loc->y>(SCREEN_HEIGHT-65)) {
			testy += -16;
		}
	}
	return (g->loc->y>(SCREEN_HEIGHT-65));
}

/*tests graphical object class*/
void CMyGame::testGob(){
	this->gobs->load("./data/testicon.bmp", "test");
	this->gobs->draw(this->screen, "test");
	CGob::gob *g = this->gobs->find("test");
	g->loc->x = 288;
	g->loc->y = 208;
	SDL_Flip(screen);
}

void CMyGame::testZounds(){
	int snd;
	double  notes[] = {
		261.626, 
		293.665, 
		329.628,
		0
	};
	cout <<"testZounds" <<endl;
	znd = new Zounds();
	double note;
	int i=0;
	Mix_Chunk * wave = NULL;
	while ((note=notes[i])) {
		i++;
		cout << note <<endl;
		//znd->playMusic(std::string("./data/closer.mod"));
		wave = znd->genSine(1, note, true);
		wave->volume = 63;
		snd = znd->playSound(wave);
		//snd = znd->playSound("./data/BONK.WAV");
		while(0 != Mix_Playing(snd));	
		free(wave); 
	}
	
}
/* tests we can load files */

/*void CMyGame::testIcon(){
	SDL_Surface *screen = this->screen;
	SDL_SetClipRect(screen, NULL);
	SDL_Surface *icon = SDL_LoadBMP(".\\data\\testicon.bmp");
	SDL_Rect * dest = new SDL_Rect;
	dest->x = 100;
	dest->y = 100;
		SDL_BlitSurface(icon, NULL, screen, dest);
	
	delete dest;
	SDL_Flip(screen);
}*/

/*tests we can render on screen*/ /*
void CMyGame::testScreen(){
	SDL_Surface *screen = this->screen;
	SDL_SetClipRect(screen, NULL);

	int status = SDL_LockSurface(screen);
	if (status==0){
		cout << "screen locked" << endl;
		Uint32 color = SDL_MapRGBA(screen->format, 255, 128, 128, 255);
		SDL_Rect *rectangle = new SDL_Rect;
		rectangle->x = 100;
		rectangle->y = 100;
		rectangle->w = 150;
		rectangle->h = 150;
		if (0!=SDL_FillRect(screen, rectangle, color)){
			cout << "fillrect fail" << endl;
		}
		delete rectangle;
		int y = 479;
		int x = 639;
		Uint32 * pixels  = (Uint32*) screen->pixels + (640*y) + (x);
		*pixels = color;
		SDL_UnlockSurface(screen);
		SDL_Flip(screen);
		//refresh surface?
	} else {
		cout << "status: " << status << endl;
	}

} */


/*
 * Initialize
 */

void CMyGame::initialize(){
		
	// Initialize defaults, Video and Audio 
	if((SDL_Init(SDL_INIT_EVERYTHING )<0)) { 
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}
	
	//SDL_Surface *screen;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS, SDL_HWSURFACE|SDL_ANYFORMAT);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		myWaitEvent();
		exit(1);
	}
	this->gobs = new CGob();

	this->gobs->create("console");
/*	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}    
*/	
}

/**
 * For now, unused callback
 */

Uint32 CMyGame::myCallback(Uint32 interval, void *param){
	SDL_Event event;
	SDL_UserEvent userevent;
	userevent.type = SDL_USEREVENT;
	userevent.code = 1;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type=SDL_USEREVENT;
	event.user=userevent;
	SDL_PushEvent(&event);
	return interval;
}
void CMyGame::writeText(std::string maitext, int size, SDL_Color color){
	/* font start*/
	TTF_Font *font;
	font=TTF_OpenFont("data\\FreeMono.ttf", 14);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	/*do stuff with font*/

	if (font) {
		this->gobs->create("text");
		this->gobs->setIcon("text", 
				TTF_RenderText_Blended(font, maitext.c_str(), color));
		TTF_CloseFont(font);
		font=NULL; // to be safe...
	}
	/* font end */

}

/***********************************************************************
*Desc: The game's Main Loop Event
*
*By: Esa Karjalainen, 04. June, 2005
***********************************************************************/

/* FIXME: Siivoa ja refaktoroi jo kovasti! */
void CMyGame::mainLoop(){
	
	//std::string text = "nou neulines,\n mou";
	//SDL_Color color;
	//color.r = 0; color.g=255; color.b=64;
	//this->writeText(text, 14, color);
	static int ticks;
	int now = SDL_GetTicks();
	ticks = now + 33;
	bool bFlagQuit = false;
	SDL_Event event;

	CGob::gob *g = this->gobs->find("test");

	//SDL_TimerID timer = SDL_AddTimer(33, myCallback, this);
	InputHandler input;

	enum actions {JUMP, LEFT, RIGHT, CONSOLE, CK, QUIT};

	Console my_console;
	my_console.setup(600, 400, "data\\FreeMono.ttf");
//	my_console.write("haaaaaa");
//	this->gobs->setIcon("console", my_console.get());
//	this->gobs->draw(this->screen, "console");
//	bool consoleMode = false;
/*	my_console.writeln("dumptest line1");
	my_console.writeln("dumptest line2");
	my_console.writeln("dumptest line3");
	my_console.writeln("dumptest line4");
	my_console.writeln("              ");

	my_console.setScreen(this->screen);
	my_console.dump();
*/
	// name, id, sdl_event, value
	input.registerAction("quit", QUIT, SDL_QUIT, 0);	
	input.registerAction("jump", JUMP, SDL_KEYDOWN, SDLK_UP);
	input.registerAction("left", LEFT, SDL_KEYDOWN, SDLK_LEFT);
	input.registerAction("right", RIGHT, SDL_KEYDOWN, SDLK_RIGHT);
	input.registerAction("quit", QUIT, SDL_KEYDOWN, SDLK_ESCAPE);
	input.registerAction("console", CONSOLE, SDL_KEYDOWN, SDLK_TAB);

	input.registerAction("close_console", CK, SDL_KEYUP, SDLK_TAB);

	input.registerAction("jjump", JUMP, SDL_JOYAXISMOTION, input.JOYUP);
	input.registerAction("jlef", LEFT, SDL_JOYAXISMOTION, input.JOYLEFT);
	input.registerAction("jright", RIGHT, SDL_JOYAXISMOTION, input.JOYRIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	while ( ( SDL_PollEvent(&event) || bFlagQuit == false)) {
		int max_speed = 6;
		now = SDL_GetTicks();
		if(now>ticks) {
			ticks = now+33; 
		} else {
			SDL_Delay(ticks-now);
		}

		int mleft=0, mright=0, jump=0; /* move 'til keyup */
		//a bit of inertia... if on ground
		if (fabs(testx)>0.9 && g->loc->y>(SCREEN_HEIGHT-65)) { 
			testx = (testx-(0.1*testx/fabs(testx)));
		} else if (fabs(testx)>0.9) { // air resistance
			testx = (testx-(0.03*testx/fabs(testx)));
		}
		testy = testy+0.5;
		g->move->x = static_cast<int>(testx);
		g->move->y = static_cast<int>(testy);
		int action = input.queryEvent(&event);
		switch (action) {		
			case QUIT:
				bFlagQuit = true;
				printf("quit\n");
				break;					
			case LEFT:
				testGobMove(action);
				break;
			case RIGHT:
				testGobMove(action);
				break;
			case JUMP:
				if (testGobMove(action)) {					
					znd->playSound("./data/BONK.WAV");				
				}
				break;
			case CONSOLE:
				if(!my_console.active){
				my_console.setScreen(this->screen);
				my_console.activate();
				action = 0;
				} 
				break;
			case CK:
				if(my_console.active){
					my_console.active=false;
				}
				break;
		}

		SDL_FillRect(this->screen, NULL, SDL_MapRGBA(this->screen->format, 0,0,0,0));
		if ( 1 == mleft  ) {
			SDL_Rect lcorner;
			lcorner.x = 0;
			lcorner.y = 0;
			lcorner.w = 20;
			lcorner.h = 20;
			if (g->move->x>-max_speed) {
				testx = testx - 1;
			} else {
				testx = -max_speed;
			}
			SDL_FillRect(this->screen, &lcorner, SDL_MapRGB(this->screen->format, 255,255,255));
		}
		if ( 1 == mright) {
			if (g->move->x<max_speed) {
				testx = testx + 1;
			} else {
				testx = max_speed;
			}
		}
		if ( 1 == jump ) {
			if (g->move->y>-max_speed) {
				testy = testy - 8;
			} else {
				testy = -max_speed;
			}
			if (g->loc->y<(SCREEN_HEIGHT-66)) {
				jump = 0;
			}
		}
		this->gobs->move("test");

		int x = g->loc->x;
		int y = g->loc->y;
		if (x>(SCREEN_WIDTH-64) || x<15 ){
			testx = 0;
			if (x<15) {
				g->loc->x=15;
			} else {
				g->loc->x = SCREEN_WIDTH-64;
			}
		} 
		if(y>(SCREEN_HEIGHT-64) || y<15){
			testy=0;
			if (y<15) {
				g->loc->y=15;
			} else {
				g->loc->y = SCREEN_HEIGHT-64;
	}
		}

		this->gobs->draw(this->screen, "test");
	//	this->gobs->draw(this->screen, "text");
//		this->gobs->setIcon("console", my_console.get());
//		this->gobs->draw(this->screen, "console");
		//my_console.dump();

		SDL_Flip(this->screen);
		}
	}


	//I think this is junk...
	void CMyGame::myWaitEvent(){
		SDL_Event event;
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				printf("The %s key was pressed!\n",
						SDL_GetKeyName(event.key.keysym.sym));
				break;
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;	
		}
	}

	//EOF
