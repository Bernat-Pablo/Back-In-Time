#ifndef _FONT_UI_
#define _FONT_UI_

#include "p2Point.h"
#include "UI_Elements.h"

class Font_UI : public UI_Elements {
public:
	Font_UI();
	~Font_UI() {};

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	iPoint position;
	char* text;
};

#endif // !_FONT_UI_
