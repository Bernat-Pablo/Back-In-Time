#ifndef _UI_ELEMENTS_
#define _UI_ELEMENTS_

#include "j1Gui.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

class UI_Elements : public j1Gui {
public:
	UI_Elements();
	UI_Elements(UI_Types type);
	virtual ~UI_Elements() {};
	
	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
public:
	char* text_font;
	SDL_Rect rect;
	char* texture_path;
	iPoint pos;
	bool following_pj;

	int numberLetters;
	iPoint margeButton;
};

enum Color {
	Black,
	White,
};

#endif // !_UI_ELEMENTS_
