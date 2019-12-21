#ifndef _UIELEMENT_
#define _UIELEMENT_

#include "j1Gui.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Fonts.h"

struct SDL_Texture;
class j1Fonts;


class UI_Element : public j1Gui
{
public:
	UI_Element();
	~UI_Element() {};

public:
	j1Fonts* fontModule;
	iPoint position;
	char* text;

};



#endif // !_UIELEMENT_
