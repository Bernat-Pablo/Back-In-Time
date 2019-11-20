#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Player.h"

class Entity;

enum timeState
{
	NO_STATE,
	NORMAL,
	SLOWING,
	STOPPED,
	FASTENING,
	RECENTLY_STOPPED
};

class EntityManager : public j1Module
{
public:
	EntityManager();
	~EntityManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
};

#endif // !__j1ENTITYMANAGER_H__
