#ifndef _MENU_
#define _MENU_

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"

enum MenuStates
{
	MAIN_MENU,
	INGAME,
	NONE,
};

class j1Menu : public j1Module {
public:
	j1Menu();
	~j1Menu() {};
	//brofile to do
	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool menuAble = true;
	bool transition = false;

	void ChangeMenuStatus(p2SString mode);
	void CreateAllUIElements();
	void DestroyAllUIElements();
	bool config = false;

	void CreateMenu();
	void DestroyMenu();

	void CreateSettings();
	void DestroySettings();

	void CreateInGameMenu();
	void DestroyInGameMenu();
public:
	MenuStates menuState;
private:
	SDL_Texture* background;
	bool menu_created;
	bool settings_created;
	bool ingame_created;
};

#endif // !_MENU_
