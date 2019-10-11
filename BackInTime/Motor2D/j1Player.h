#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "j1Animation.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Character.h"
#include "j1Wot.h"

struct SDL_Texture;
struct Mix_Chunk;


class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

private:

};

#endif // !_PLAYER_
