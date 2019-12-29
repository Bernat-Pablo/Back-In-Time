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

private:


};

#endif // !_SLIDER_
