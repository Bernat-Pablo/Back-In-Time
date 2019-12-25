#ifndef _GUI_
#define _GUI_

#include "j1Module.h"
#include "p2List.h"

class UI_Elements;

enum class UI_Type{
	TEXT,
};

class j1Gui : public j1Module {
public:
	j1Gui();
	~j1Gui() {};

	UI_Elements* CreateEntity(UI_Type type, int position_x, int position_y);


private:
	
};

#endif // !_GUI_
