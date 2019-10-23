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
	void hability();
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

	SDL_Texture* spritesheet_pj = nullptr;

public:
	Player_States state;
	Player_Input player_input; //Input introduced by the player
	int health = 3;


	float initial_x = 32.0f; // 2 blocks * 16 each one
	float initial_y = 400.0f; // 27 blocks * 16 each one - height pj	
	iPoint position; //Use position.x and position.y
	iPoint old_position;

	float decrease_vel = 0.2f;
	float velocity = 2.0f;

	float run_velocity = 3.0f;
	float fall_velocity = 0.0f;
	float gravity = 6.0f;
	float jump_vel = 6.5f;

	bool moving_right = false; //for slide
	bool moving_left = false;	
	bool in_air = false;

	bool collider_at_right = false; //player can't go to the right
	bool collider_at_left = false;

	bool godMode = false;

	Collider* collider_player = nullptr;

	Collider* camera_toRight = nullptr;
	Collider* camera_toLeft = nullptr;
	Collider* camera_toUp = nullptr;
	Collider* camera_toDown = nullptr;

};



#endif // !_PLAYER_
