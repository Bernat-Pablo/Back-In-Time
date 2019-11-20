#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

class Entity
{
public:
	enum entityType
	{
		PLAYER,
		FLYING_ENEMY,
		GROUND_ENEMY,
	};

public:
	Entity();
	Entity(entityType type);
	~Entity();

	void Draw(float dt);
};

#endif // !__j1ENTITY_H__