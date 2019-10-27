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
	void checkAbility();
	void useAbility();

	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);
	void restart_variables(int vel, int jump_vel);

	void MoveCameraColliders(p2SString direction, float speed);
	bool checkInAir();

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

	SDL_Texture* spritesheet_pj = nullptr;
	const char* spritesheet_source;

	Player_States state; //Current player state
	Player_Input player_input; //Input introduced by the player
	int lives = 3;

	p2SString			folder;
	float initial_x = 200.0f; // 2 blocks * 16 each one
	float initial_y = 400.0f; // 25 blocks * 16 each one - height pj	
	
	iPoint old_position[15];
	int iterator = 0;

	float decrease_vel = 0.2f;
	float velocity = 2.0f;

	float run_velocity;
	float fall_velocity;
	float gravity;
	float jump_vel;

	bool moving_right = false; //for slide
	bool moving_left = false;	
	bool in_air = false;
	bool looking_right=true;

	bool collider_at_right = false; //If true, player can't go to the right 
	bool collider_at_left = false;

	bool godMode = false;
	bool walking = false;

	bool ability_able = false;

	int tick1 = 0, tick2 = 0;
	int tick3 = 0, tick4 = 0;

	Collider* collider_player = nullptr;

	Collider* camera_toRight = nullptr;
	Collider* camera_toLeft = nullptr;
	Collider* camera_toUp = nullptr;
	Collider* camera_toDown = nullptr;

	pugi::xml_document doc;
	pugi::xml_node node;
};

#endif // !_PLAYER_
