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


};


#endif // !_INPUTTEXT_
