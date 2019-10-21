#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "j1Map.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player() {};

	bool Start();
	bool Update(float dt);

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
	float initial_x = 32.0f;
	float initial_y = 350.0f;
	float x = initial_x;  // 2 blocks * 16 each one
	float y = initial_y; // 27 blocks * 16 each one - height pj

	float decrease_vel = 0.1f;
	float velocity = 2.0f;
	float run_velocity = 2.05f;
	float gravity = 2.0f;

	bool moving_right = false;
	bool moving_left = false;	

};



#endif // !_PLAYER_
