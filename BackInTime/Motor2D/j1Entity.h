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
	DASH_FORWARD, //Slowly stops the entity 
	DASH_BACKWARD,
	HIT,
	DIE,

	FLY,
	FLY_FORWARD,
	FLY_BACKWARD,
	FLY_UP,
	FALL,
	IN_GROUND,

	STUNNED,
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
	virtual bool OnCollision() { return true; };

	virtual void BlitEverything() {};
	
public:
	Animation idle;
	Animation walk;
	Animation run;
	Animation stunning;
	Animation death;
	Animation throw_rock;
	Animation jump_up;
	Animation jump_down;
	Animation hurt;
	Animation fly;
	Animation ground;
	Animation hit;
	Animation fall;
	Animation* current_animation;

	SDL_Texture* spritesheet_entity;
	Collider* collider_entity;	

	//Type and states
	entityTypes type = entityTypes::UNKNOWN;
	entityStates state = entityStates::IDLE;
		
	//Initial position
	float				initial_x;
	float				initial_y;	
	
	bool				isDead;
	bool				moving_right;
	bool				moving_left;
	bool				isgrounded;
	bool				being_hit;
	bool				reversed;

	//Entity settings
	iPoint				position;
	float				velocity;
	float				run_velocity;
	float				fall_velocity;
	float 				max_fall_velocity;
	float 				gravity;
	float 				jump_vel;
	float				decrease_vel;

	float				deltaTime;

	//Colliders and collisions
	bool 				collider_at_right; //If true, entity can't go to the right 
	bool 				collider_at_left;
};

#endif // !__j1ENTITY_H__