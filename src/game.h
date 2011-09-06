#pragma once 

#include "SDL.h"
#include "gob.h"
#include "Console.h"
#include "Zounds.h"
#include <vector>
#include <string>
#include "SDL_ttf.h"

/*externals*/
//#include "../ext/StickyInput.h"

class CMyGame{

public:
	//constructor, destructor
	CMyGame();
	~CMyGame();

	//methods
	void runGame();

private:
	CGob* gobs;
	Zounds * znd;
	SDL_Surface * screen;
	void initialize();
	void myWaitEvent();
	void mainLoop();
	//timer callback

	//test functions
	/*void drawGOB(std::string name);
	void loadGOB(std::string bitmap, std::string name);
	void moveGOB(std::string, int, int);
	CMyGame::gob * findGOB(std::string name);
	void moveGOB(std::string);*/
	bool testGobMove(int action);
	static Uint32 myCallback(Uint32 interval, void *param);

	void writeText(std::string maitext, int size, SDL_Color color);
	void testGob();
	void testZounds();
	void testScreen();
	void testIcon();
	float testx;
	float testy;
};
