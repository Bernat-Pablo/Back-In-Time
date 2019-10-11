#ifndef _WOT_
#define _WOT_


#include "j1Character.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Player.h"


struct SDL_Texture;

class Wot : public j1Character
{
public:
	Wot();
	~Wot() {};
	bool Start() override;
	bool CleanUp() override;

};


#endif // !_WOT_
