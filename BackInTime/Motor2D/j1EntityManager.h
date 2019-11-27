#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Player.h"

class j1Entity;

enum class entityTypes
{
	PLAYER,
	FLYING_ENEMY,
	WALKING_ENEMY,
};

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	virtual bool Awake(pugi::xml_node& config);
	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	j1Entity* CreateEntity(entityTypes type, int position_x, int position_y);

	//TODO 
	//Function CreateEntity
	//Function DestroyEntity
	//Entities list
public:
	p2List<j1Entity*> entitiesList;
};

#endif // !__j1ENTITYMANAGER_H__