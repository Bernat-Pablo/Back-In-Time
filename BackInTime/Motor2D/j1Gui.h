#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "p2List.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

enum class UI_TypeElement {
	BUTTON,
	IMAGE,
	INPUT_TEXT,
	SCROLL_BAR,
	TEXT,
};

class UI_Element;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui() {};

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	UI_Element* CreateUI_Element(UI_TypeElement type, int position_x, int position_y); //hacer funciones para boton, text, etc. y despues a�adirlo a la lista de elementos
	void destroyUI_Element(j1Gui* element);

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	//crear lista de elementos ui

	p2List<UI_Element*> ui_list;

};

#endif // __j1GUI_H__