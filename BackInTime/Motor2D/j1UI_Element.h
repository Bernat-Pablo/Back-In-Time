#ifndef _UIELEMENT_
#define _UIELEMENT_

#include "j1Gui.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

struct SDL_Texture;

enum class UI_TypeElement {
	BUTTON,
	IMAGE,
	INPUT_TEXT,
	SCROLL_BAR,
	TEXT,
};

class UI_Element : public j1Gui
{
public:
	UI_Element();
	~UI_Element();

private:

};



#endif // !_UIELEMENT_
