#ifndef _SLIDER_
#define _SLIDER_

#include "UI_Elements.h"
#include "p2Point.h"

class Slider : public UI_Elements {
public:
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool On();
	bool OnClick();

private:
	SDL_Rect base;
	SDL_Rect quad;

	iPoint mouse;

	int slider;
};


#endif // !_SLIDER_
