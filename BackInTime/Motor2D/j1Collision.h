#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 50

#include "j1Module.h"
#include "j1App.h"
#include "SDL/include/SDL.h"

enum COLLIDER_TYPE
{
	COLLIDER_PLAYER,
	COLLIDER_WALL,
};

struct Collider
{
	SDL_Rect rect;
};

class j1Collision : public j1Module
{
public:
	j1Collision();
	~j1Collision();

	bool CleanUp() override;

	
private:
	
	
};

#endif // __ModuleCollision_H__