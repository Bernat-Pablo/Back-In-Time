#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

struct SDL_Texture;
struct Collider;

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

class j1Entity : public j1EntityManager
{
public:
	j1Entity(entityTypes type);
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };	
	
public:
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

	iPoint position; //Use position.x and position.y
	SDL_Texture* texture = nullptr;

	entityTypes type = entityTypes::UNKNOWN;
	entityStates state = entityStates::IDLE;
};

#endif // !__j1ENTITY_H__