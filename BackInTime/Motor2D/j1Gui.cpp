#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_Elements.h"

j1Gui::j1Gui()
{
}

UI_Elements* j1Gui::CreateEntity(UI_Type type, int position_x, int position_y)
{
	UI_Elements* ret = nullptr;
	switch (type)
	{
	case UI_Type::TEXT:
		ret = new j1Fonts();
		break;
	default:
		break;
	}
	return ret;
}
