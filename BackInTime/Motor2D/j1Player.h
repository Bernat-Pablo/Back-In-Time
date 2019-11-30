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

	void MoveCameraColliders(p2SString direction, float speed);
	bool checkInAir();

	void rockMovement();
	void throwRock();
	bool rockCheckInAir();

	iPoint position;	
private:
	
	SDL_Rect bar_0, bar_1, bar_2, bar_3, bar_4;
	iPoint bar_pos;

	SDL_Texture* spritesheet_rock = nullptr;
	SDL_Texture* spritesheet_casper = nullptr;
	SDL_Texture* spritesheet_bars = nullptr;
	const char* spritesheet_source;

	Player_Input player_input; //Input introduced by the player	

	p2SString			folder;
	
	iPoint				old_position[30];
	int					iterator = 0;
	int					position_when_ability;
	int					initial_pos;
	int					screen_size;	

	bool 				in_air = false;
	bool 				looking_right=true;	

	bool 				godMode = false;
	bool 				walking = false; //is he walking?

	bool 				ability_able = false;

	//Rock data
	iPoint				rockPosition;
	iPoint				rockVelocity;
	bool 				rock_able = true;
	float				rock_timer = 0;
	float				rock_cooldown;
	float				rock_fall_velocity = 0;

	int 				tick1 = 0, tick2 = 0;
	int 				tick3 = 0, tick4 = 0;

	Collider*			camera_toRight = nullptr;
	Collider*			camera_toLeft = nullptr;
	Collider*			camera_toUp = nullptr;
	Collider*			camera_toDown = nullptr;

	Collider*			collider_rock = nullptr;

	pugi::xml_document	doc;
	
};

#endif // !_j1PLAYER_H_
