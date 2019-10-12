#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 50

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_PLAYER,
	COLLIDER_WALL,
	COLLIDER_TERRAIN,
};

class j1Collision : public j1Module
{
public:
	j1Collision();
	~j1Collision();

	bool CleanUp() override;

	
private:
	//bool colliders[10][10];	
	
};

#endif // __ModuleCollision_H__