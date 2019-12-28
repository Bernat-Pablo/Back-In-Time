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
	bool CleanUp();
	void Draw();
	void onClick();

private:

	char* text;
	SDL_Rect r;

};

#endif // !_BUTTON_
