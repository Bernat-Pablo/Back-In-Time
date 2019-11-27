#ifndef _FLYINGENEMY_
#define _FLYINGENEMY_

#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Collider;

enum FEnemy_States {
	FLY,
	FLY_FORWARD,
	FLY_BACKWARD,
	FALL,
	IN_GROUND,
};


class j1FlyingEnemy : public j1Module
{
public:
	j1FlyingEnemy();
	~j1FlyingEnemy() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void calculate_path();
	void blit_path();
	void check_path_toMove();

	float x_pos, y_pos;

private:

	Animation fly;
	Animation ground;
	Animation hit;
	Animation fall;
	Animation* current_animation;

	FEnemy_States state;
	bool moving_right = false;
	bool moving_left = false;
	bool falling = false;

	Collider* collider_enemy = nullptr;

	float deltaTime;

	float velocity;
	float fall_vel;

	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* debug_tex = nullptr;

	int path_num = 0;

};

#endif // !_FLYINGENEMY_
