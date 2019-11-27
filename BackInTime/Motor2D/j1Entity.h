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

	iPoint position; //Use position.x and position.y
private:
	Animation idle;
	Animation walk;
	Animation run;
	Animation death;
	Animation throw_rock;
	Animation jump_up;
	Animation jump_down;
	Animation hurt;
	Animation* current_animation;

	Collider* collider_entity = nullptr;
};

#endif // !__j1ENTITY_H__