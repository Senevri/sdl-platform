#pragma once
/**
 * Console class:
 * Hooks to keyboard, echos typed keys on screen. 
 * On enter, processes input and switches to a new line
 */
#include "SDL.h"
#include "SDL_ttf.h"

#include "InputHandler.h"
#include <vector>
#include <string>

class Console{
public:
	Console();
	~Console();
	void setup(int width, int height, std::string font);
	SDL_Surface * get();
	void activate();
	void dump();
	void writeln(std::string);
	void write(std::string);
	void setScreen(SDL_Renderer * window);
	void draw();
	bool active;
private:
	void writeText(const Uint16 * text, int size, SDL_Color color);
	SDL_Surface * m_psurface;
	std::string m_font;
	SDL_Color color; 
	SDL_Renderer * m_renderer;
	InputHandler m_input;
	std::vector<Uint16 *> m_lines;
	void drawCursor(int i);
	
};

