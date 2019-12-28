#ifndef _GUI_
#define _GUI_

#include "j1Module.h"
#include "p2List.h"
#include "j1Textures.h"

class UI_Elements;

enum class UI_Types {
	TEXT,
	IMAGE,
};

class j1Gui : public j1Module {
public:
	j1Gui();
	~j1Gui() {};

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	UI_Elements* CreateUIElement(UI_Types type, int position_x, int position_y, char* texture = "0", bool follow_pj = true, char* t = "0");
	
	SDL_Texture* GetUISpritesheet();
private:
	p2List<UI_Elements*> ui_elementsList;
	SDL_Texture* ui_spritesheet;
	p2SString ui_spritesheet_path;
};

#endif // !_GUI_
