#include "InputHandler.h"

InputHandler::InputHandler(void)
{	
	printf("inputhandler constructor\n");	
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
	if(SDL_NumJoysticks() > 0) {
		this->m_pjoystick = SDL_JoystickOpen(0);
	}

}

InputHandler::~InputHandler(void)
{
	if(this->m_pjoystick != nullptr){
		SDL_JoystickClose(this->m_pjoystick);
	}
}

/* registerAction:
 * name: some name we wanna call it
 * id: id we recognize this action as - typically enumerated.
 * sdl_event_type: from SDL_Event.type
 * value: a specific char / button / data we expect
 */
void InputHandler::registerAction(const std::string name, 
		unsigned int id, 
		SDL_EventType sdl_event_type, 
		Uint16 value)
{
	action a;
	a.id = id;
	a.name = name;
	a.sdl_event_type = sdl_event_type;
	a.value = value;
	this->actions.push_back(a);
}
void InputHandler::deRegister(const std::string name){
	std::vector<action>::iterator itr;
	for ( itr = this->actions.begin(); itr < this->actions.end(); ++itr ){
		if(0==itr->name.compare(name)){
			this->actions.erase(itr);
			break;
		}
	}
}


/*TODO: IN PROGRESS!!!1one*/
int InputHandler::queryEvent(const SDL_Event *event){	
	int deadzone = 8000;
	int motion=-1;
	for (auto itr = this->actions.begin(); itr < this->actions.end(); ++itr ){
		if(event->type == itr->sdl_event_type){
			switch(event->type){
			case SDL_KEYUP:
				if(itr->value==event->key.keysym.sym)
					return itr->id;
				break;
			case SDL_KEYDOWN:
				if(itr->value==event->key.keysym.sym)
					return itr->id;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_JOYBUTTONUP:
				break;
			case SDL_JOYBUTTONDOWN:
				if(itr->value==event->jbutton.button)
					return itr->id;
				break;
			case SDL_JOYAXISMOTION:
				if(1 == event->jaxis.axis){
				if(event->jaxis.value < -deadzone)
					motion = JOYUP;
				if(event->jaxis.value > deadzone)
					motion = JOYDOWN;
				}else if (0 == event->jaxis.axis ){
				if(event->jaxis.value < -deadzone)
					motion = JOYLEFT;
				if(event->jaxis.value > deadzone)
					motion = JOYRIGHT;
				}
				if(itr->value == motion){
					return itr->id;
				}			
				break;
			case SDL_JOYBALLMOTION:
				break;
			case SDL_JOYHATMOTION:
				break;
			default:
				return itr->id;
			}
		} //if
	}//for
	return -1;  /* no action found */
}
