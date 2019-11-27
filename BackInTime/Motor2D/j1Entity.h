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
	enum class entityStates
	{
		IDLE,
		WALK_FORWARD,
		WALK_BACKWARD,
		RUN_FORWARD,
		RUN_BACKWARD,
		JUMP,
		JUMP_FORWARD,
		JUMP_BACKWARD,
		DASH_FORWARD, //Slowly stops the player
		DASH_BACKWARD,
	};

public:
	j1Entity(entityTypes type);
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	
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