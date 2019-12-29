#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Entity.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct Player_Input {
	bool pressing_W;
	bool pressing_A;
	bool pressing_S;
	bool pressing_D;
	bool pressing_F;
	bool pressing_lshift;
	bool pressing_space;
};

class j1Player : public j1Entity
{
public:
	j1Player();
	virtual ~j1Player() {};

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

	void checkCameraColliders(Collider* c1, Collider* c2);
	void MoveCameraColliders(p2SString direction, float speed);
	bool checkInAir();

	void rockMovement(float dt);
	void throwRock();
	bool rockCheckInAir();

	void ApplyForces(float dt);
	void BlitEverything();
	iPoint position;	

	void CollectCoin();
	Collider*			camera_toRight;
	Collider*			camera_toLeft;
	bool 				godMode;

public:
	bool				continue_button;
	int					collected_coins;
	int					lives;
private:
	
	SDL_Rect			bar_0, bar_1, bar_2, bar_3, bar_4;
	iPoint				bar_pos;

	SDL_Texture*		spritesheet_rock;
	SDL_Texture*		spritesheet_casper;
	SDL_Texture*		spritesheet_bars;
	const char*			spritesheet_source;

	Player_Input		player_input; //Input introduced by the player	

	p2SString			folder;
	
	iPoint				old_position[30];
	int					iterator = 0;
	int					position_when_ability;
	int					initial_pos;
	int					screen_size;	

	bool 				in_air;
	bool 				looking_right;	

	bool 				walking; //is he walking?

	bool 				ability_able;

	//Rock data
	iPoint				rockPosition;
	iPoint				rockVelocity;
	bool 				rock_able = true;
	float				rock_timer;
	float				rock_cooldown;
	float				rock_fall_velocity;
	float				rock_gravity;

	int 				tick1, tick2;
	int 				tick3, tick4;

	Collider*			camera_toUp;
	Collider*			camera_toDown;

	Collider*			collider_rock;

	pugi::xml_document	doc;
	
	bool				livesUpdated;	

	float				score;

	float				game_timer;
};

#endif // !_j1PLAYER_H_
