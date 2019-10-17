#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	void OnCollision(Collider* c1, Collider* c2);
public:

	Animation idle;
	Animation walk;
	Animation run;
	Animation death;
	Animation throw_rock;
	Animation jump;
	Animation hurt;
	Animation* current_animation;

	SDL_Texture* spritesheet_pj = nullptr;

public:

	int health = 3;
	float x = 32.0f;  // 2 blocks * 16 each one
	float y = 350.0f; // 27 blocks * 16 each one - height pj

	float decrease_vel = 0.1f;
	float velocity = 2.0f;
	float run_velocity = 2.05f;
	float gravity = 3.0f;

	bool moving_right = false;
	bool moving_left = false;	

	Collider* collider = nullptr;
};



#endif // !_PLAYER_
