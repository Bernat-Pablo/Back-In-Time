#ifndef _BUTTON_
#define _BUTTON_

#include "UI_Elements.h"
#include "p2Point.h"

class Button : public UI_Elements {
public:
	Button();
	~Button() {};

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool On();
	bool OnClick();

private:

	char* text;
	SDL_Rect r;
	iPoint text_marge;

	iPoint mouse;
};

#endif // !_BUTTON_
