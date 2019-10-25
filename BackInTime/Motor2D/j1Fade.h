#ifndef _J1FADE_
#define _J1FADE_

#include "j1Module.h"
#include "j1Audio.h"
#include "SDL\include\SDL_rect.h"

class j1Fade : public j1Module
{
public:
	j1Fade() {};
	~j1Fade();

	bool Start();
	bool Awake(pugi::xml_node& c);
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool IsFading = false;

private:

	j1Module* moduleOff = nullptr;
	j1Module* moduleOn = nullptr;


	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;

	pugi::xml_document doc;
	pugi::xml_node node;


};

#endif //_J1FADE_