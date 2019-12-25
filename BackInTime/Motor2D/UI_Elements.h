#ifndef _UI_ELEMENTS_
#define _UI_ELEMENTS_

#include "j1Gui.h"

class UI_Elements : public j1Gui {
public:
	UI_Elements(UI_Types type);
	virtual ~UI_Elements() {};

private:

	char* text = nullptr;
};

#endif // !_UI_ELEMENTS_
