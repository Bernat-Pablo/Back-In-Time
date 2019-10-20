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

struct Player_Input {
	bool pressing_W;
	bool pressing_A;
	bool pressing_S;
	bool pressing_D;
	bool pressing_lshift;
	bool pressing_space;
};

enum Player_States {
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

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

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
	Player_States state;
	Player_Input player_input; //Input introduced by the player
	int health = 3;
	
	// x = 2 blocks * 16 each one
	// y = 27 blocks * 16 each one - height pj
	iPoint position;
	iPoint lastPosition; //Useful for collisions

	float decrease_vel = 0.1f;
	float velocity = 2.0f;
	float run_velocity = 2.05f;
	float gravity = 3.0f;

	bool can_move_right = true; //for collisions
	bool can_move_left = true;
	bool moving_right = false;
	bool moving_left = false;	

	bool isGrounded = false; //Player is touching the ground

	Collider* collider_player = nullptr;
};



#endif // !_PLAYER_
