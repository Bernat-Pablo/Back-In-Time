#ifndef _MENU_
#define _MENU_

#include "j1Module.h"
#include "j1Textures.h"

class j1Menu : public j1Module {
public:
	j1Menu();
	~j1Menu() {};
	//brofile to do
	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	SDL_Texture* background;

#endif // !_MENU_
