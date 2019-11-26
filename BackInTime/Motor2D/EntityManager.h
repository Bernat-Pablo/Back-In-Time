#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"

class Entity;

enum Entity_Types {
	PLAYER,
	WALKING_ENEMY,
	FLYING_ENEMY,
};

class EntityManager : public j1Module
{
public:
	EntityManager();
	~EntityManager();

	virtual bool Awake(pugi::xml_node& config);
	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	//TODO 
	//Function CreateEntity
	//Function DestroyEntity
};

#endif // !__j1ENTITYMANAGER_H__