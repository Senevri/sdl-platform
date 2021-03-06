#include "Console.h"
#include "SDL_ttf.h"
#include <ostream>

Console::Console()
{
	if(0==TTF_WasInit()){
		if(TTF_Init()==-1) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(2);
		}   
	}
	this->color.r = 0;
	this->color.g = 255;
	this->color.b = 64;
	this->active = false;
}

Console::~Console()
{
//	delete this->m_psurface;
	SDL_FreeSurface(this->m_psurface);	
	this->m_lines.clear();
}

void Console::setup(int width, int height, std::string font){
	this->m_psurface = SDL_CreateRGBSurface(SDL_HWSURFACE |SDL_SRCALPHA , width, height, 32, 0,0,0,0);
	this->m_font = font;
}

void Console::setScreen(SDL_Surface * screen){
	this->m_screen = screen;
}

/*write all lines in buffer*/
void Console::dump(){
//	for(std::vector<Uint16 *>::const_iterator itr = this->m_lines.begin(); itr!=this->m_lines.end(); itr++){
	for(unsigned int i=0; i<this->m_lines.size();i++){
		this->writeText(this->m_lines[i], 14, this->color);
	}
//	}
	this->draw();
}

void Console::draw(){
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = this->m_psurface->w;
	r.h = this->m_psurface->h;
	//SDL_FillRect(this->m_screen, &r, SDL_MapRGBA(this->m_screen->format, 0,0,0,0));
	//SDL_SetClipRect(this->m_screen, NULL);
	SDL_BlitSurface(this->m_psurface, &r, this->m_screen, &r);
}

SDL_Surface * Console::get(){
	return this->m_psurface;
}

/* write to console buffer*/
void Console::writeln(std::string str){
	std::wstring ustr(str.length(), L' ');
	std::copy(str.begin(), str.end(), ustr.begin());
	//str.append(1, 0);
	Uint16 * line = new Uint16[ustr.size()];
	std::copy(ustr.begin(), ustr.end(), line);
	//Uint16 * line = (Uint16 *)ustr.data();
	
	this->m_lines.push_back(line);
//	writeText((const Uint16 *)ustr.data(), 14, this->color);
}

/* Blind write no store */
void Console::write(std::string str){
	std::wstring ustr(str.length(), L' ');
	std::copy(str.begin(), str.end(), ustr.begin());

	writeText((const Uint16 *)ustr.data(), 14, this->color);
}

void Console::writeText(const Uint16 * text, int size, SDL_Color color){
	wprintf((const wchar_t*)text);
	printf("\n");
	/* font start*/
	SDL_Surface * tempsurface = 0;
	TTF_Font *font;
	font=TTF_OpenFont(this->m_font.c_str(), size);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	/*do stuff with font*/
	int height, width;
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	int mul = this->m_lines.size();
	//if (mul<1) mul=1;
	if (font) {
		TTF_SizeUNICODE(font, text, &width, &height);					
		tempsurface = TTF_RenderUNICODE_Blended(font, text, color);
		r.w = width;
		r.h = height;
		r.y = height * mul;
		//printf("%d %d %d %d\n", r.w, r.h, r.x, m_lines.size());
		SDL_FillRect(this->m_psurface, &r, SDL_MapRGBA(this->m_psurface->format, 0,0,0,0));
		SDL_BlitSurface(tempsurface, NULL, this->m_psurface, &r);
		TTF_CloseFont(font);
		font=NULL; // to be safe...
		}
	delete (tempsurface);
	/* font end */
}

void Console::drawCursor(int i){
	SDL_Surface * tempsurface = 0;
	TTF_Font *font;
	font=TTF_OpenFont(this->m_font.c_str(), 14);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	/*do stuff with font*/
	int height, width;
	SDL_Rect r;
	r.x = 0;
	r.y = 0;

	if (font) {
		TTF_SizeText(font, "|", &width, &height);					
		tempsurface = TTF_RenderText_Blended(font, " ", color);
		r.h = height;
		r.x = i * width;
		r.w = this->m_psurface->w - r.x;
		r.y = height * (m_lines.size());
		//printf("%d %d %d %d\n", r.w, r.h, r.x, m_lines.size());
		SDL_FillRect(this->m_psurface, &r, SDL_MapRGBA(this->m_psurface->format, 0,0,0,0));
		SDL_BlitSurface(tempsurface, NULL, this->m_psurface, &r);
		TTF_CloseFont(font);
		font=NULL; // to be safe...
	}
	delete (tempsurface);

}

void Console::activate(){
	this->dump();
	bool isActive = true;
	active = true;
	//InputHandler input ;
	Uint16 current_line[1000];
	m_input.registerAction("console", 10001, SDL_KEYDOWN, SDLK_TAB);
	SDL_EnableUNICODE(1);
	int i = 0;
	int ticks = SDL_GetTicks();
	int now = 0;
	SDL_Event event;
	m_input.queryEvent(&event);

	while(SDL_WaitEvent(&event) && isActive) {
		if (10001 == m_input.queryEvent(&event)) {
			std::cout << "return from console" << std::endl;
			isActive = false;
			return; //may be a bad idea
		} else {
			if(event.type == SDL_KEYDOWN) {
				/* clear cursor */
				/* store character to buffer, echo to screen*/
				if(event.key.keysym.sym == SDLK_RETURN) {
					current_line[i+1] = 0;
					Uint16 * line = new Uint16[i+1];
					std::copy(current_line, current_line+i+1, line);
					this->m_lines.push_back(line);
					//should we, like, del current line here?
					current_line[0] = 0;
					i = 0;
					//std::cout << "newline" << std::endl;
					//this->writeText(current_line, 14, this->color);
				} else if (event.key.keysym.sym == SDLK_BACKSPACE){
					current_line[i]=0;
					i--;
				} else {
					//this->write(".");
					current_line[i] =(event.key.keysym.unicode);
					current_line[i+1] = L'\0';
					i++;
					this->writeText(current_line, 14, this->color);
					//this->draw();
				}
			}
		}
		this->drawCursor(i);
		this->draw();		
		SDL_Flip(this->m_screen);
	}
	m_input.deRegister("console");
	SDL_EnableUNICODE(0);
	active = false;

}
