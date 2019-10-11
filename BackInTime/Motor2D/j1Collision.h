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

class ModuleCollision : public j1Module
{
public:
	ModuleCollision();
	~ModuleCollision();



	bool CleanUp() override;

	void DebugDraw(); //Draw colliders on debug mode

private:
	bool colliders[10][10];
	bool debug = false; //If true, we draw colliders
};

#endif // __ModuleCollision_H__