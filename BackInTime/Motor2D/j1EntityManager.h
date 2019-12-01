#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"

class j1Entity;

enum class entityTypes
{
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY,
	ROCK,
	UNKNOWN,
};

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	virtual bool Awake(pugi::xml_node& config);
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	bool Save(pugi::xml_node& file)const;
	//bool Load(pugi::xml_node& file);

	j1Entity* CreateEntity(entityTypes type, int position_x, int position_y);

	//TODO 
	//Function CreateEntity
	//Function DestroyEntity
	//Entities list

	p2List<j1Entity*> entitiesList;
	j1Entity* player = nullptr;
	j1Entity* testFlyingEnemy = nullptr;
	j1Entity* testFlyingEnemy2 = nullptr;
	j1Entity* testGroundEnemy = nullptr;
};

#endif // !__j1ENTITYMANAGER_H__