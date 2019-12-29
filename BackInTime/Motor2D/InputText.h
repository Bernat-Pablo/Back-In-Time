#ifndef _INPUTTEXT_
#define _INPUTTEXT_

#include "j1Gui.h"
#include "UI_Elements.h"
#include "p2Point.h"

class InputText : public UI_Elements {
public:
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool OnClick();
		
private:
	iPoint position;
	SDL_Rect r;
	SDL_Rect line;
	int number_letters;
	int spacing;

	int tick1 = 0, tick2 = 0;
};


#endif // !_INPUTTEXT_
