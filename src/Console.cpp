#include "Console.h"
#include "Debug.h"
#include "SDL_ttf.h"
#include <ostream>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

Console::Console()
{
	if (2 == init()) {
		exit(2);
	}
	
	this->color.r = 0;
	this->color.g = 255;
	this->color.b = 64;
	this->active = false;
}

int Console::init() {
	if (0 == TTF_WasInit()) {
		if (TTF_Init() == -1) {
			Debug::Log("TTF_Init: %s\n", TTF_GetError());
			return 2;
		}
		return 0;
	}
	return 1;
}

Console::~Console()
{
	//	delete this->m_psurface;
	SDL_FreeSurface(this->m_psurface);	
	this->m_lines.clear();
}

void Console::setup(int width, int height, std::string font){
	this->m_psurface = SDL_CreateRGBSurface(0 , width, height, 32, 0,0,0,0);
	this->m_font = font;
}

void Console::setScreen(SDL_Renderer * screen){
	this->m_renderer = screen;
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
	r.w = SCREEN_WIDTH;
	r.h = SCREEN_HEIGHT;
	//SDL_FillRect(this->m_screen, &r, SDL_MapRGBA(this->m_screen->format, 0,0,0,0));
	//SDL_SetClipRect(this->m_screen, NULL);
	//SDL_BlitSurface(this->m_psurface, &r, this->m_screen, &r);
	auto tex = SDL_CreateTextureFromSurface(this->m_renderer, this->m_psurface);
	SDL_RenderCopy(this->m_renderer, tex, &this->m_psurface->clip_rect, &r);  
}

SDL_Surface * Console::get(){
	return this->m_psurface;
}

/* write to console buffer*/
void Console::writeln(std::string str){
	std::wstring ustr(str.begin(), str.end());	
	//std::copy(str.begin(), str.end(), ustr.begin());
	ustr.append(0);
	auto line = new std::vector<Uint16>();
	line->reserve(ustr.size());
	try {
		std::copy(ustr.begin(), ustr.end(), line->begin());
	} catch (std::exception ex) {
		std::cerr << "problem copying wchar_t to Uint16" <<std::endl;
		Debug::Log("problem copying wchar_t to Uint16");
	}
	

	this->m_lines.push_back(line->data());
	//	writeText((const Uint16 *)ustr.data(), 14, this->color);
}

/* Blind write no store */
void Console::write(std::string str){
	std::wstring ustr(str.begin(), str.end());	
	writeText((const Uint16 *)ustr.data(), 14, this->color);
}

void Console::writeText(const Uint16 * text, int size, SDL_Color color){
	init();
	wprintf((const wchar_t*)text);
	printf("\n");
	/* font start*/
	SDL_Surface * tempsurface = 0;
	TTF_Font *font;
	font=TTF_OpenFont(this->m_font.c_str(), size);
	if(!font) {
		Debug::Log("writeText -- TTF_OpenFont: %s\n", TTF_GetError());
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
	SDL_FreeSurface(tempsurface);
	/* font end */
}

void Console::drawCursor(int i){
	init();
	SDL_Surface * tempsurface = 0;
	TTF_Font *font;
	font=TTF_OpenFont(this->m_font.c_str(), 14);
	if(!font) {
		Debug::Log("drawCursor -- TTF_OpenFont: %s\n", TTF_GetError());
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
	SDL_FreeSurface(tempsurface);

}

void Console::activate(){
	this->dump();
	bool isActive = true;
	active = true;
	//InputHandler input ;
	Uint16 current_line[1000];
	m_input.registerAction("console", 10001, SDL_KEYDOWN, SDLK_TAB);

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
			
			if (event.type == SDL_TEXTEDITING) {
				Debug::Log(event.text.text);
				//std::copy(current_line, event.text.text);

			}

			//if(event.type == SDL_KEYDOWN) {
			//	/* clear cursor */
			//	/* store character to buffer, echo to screen*/
			//	if(event.key.keysym.sym == SDLK_RETURN) {
			//		current_line[i+1] = 0;
			//		Uint16 * line = new Uint16[i+1];
			//		std::copy(current_line, current_line+i+1, line);
			//		this->m_lines.push_back(line);
			//		//should we, like, del current line here?
			//		current_line[0] = 0;
			//		i = 0;
			//		//std::cout << "newline" << std::endl;
			//		//this->writeText(current_line, 14, this->color);
			//	} else if (event.key.keysym.sym == SDLK_BACKSPACE){
			//		current_line[i]=0;
			//		i--;
			//	} else {
			//		//this->write(".");
			//		current_line[i] =(event.key.keysym.unicode);
			//		current_line[i+1] = L'\0';
			//		i++;
			//		this->writeText(current_line, 14, this->color);
			//		//this->draw();
			//	}
			//}
		}
		this->drawCursor(i);
		this->draw();		
		SDL_RenderPresent(this->m_renderer);
	}
	m_input.deRegister("console");	
	active = false;

}
