#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1EntityManager.h"

class j1Entity
{
public:
	enum class entityTypes
	{
		PLAYER,
		FLYING_ENEMY,
		WALKING_ENEMY,
	};

public:
	j1Entity();
	j1Entity(entityTypes type);
	~j1Entity();

	virtual void Update();
	virtual void Draw();
	virtual void HandleInput();

	iPoint position;
};

#endif // !__j1ENTITY_H__