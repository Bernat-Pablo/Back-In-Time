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

	bool Awake(pugi::xml_node& config);
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
	int x = 48;  // 3 blocks * 16 each one
	int y = 432; // 27 blocks * 16 each one 
	

};



#endif // !_PLAYER_
