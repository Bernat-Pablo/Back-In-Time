#ifndef _FLYINGENEMY_
#define _FLYINGENEMY_

#include "j1Entity.h"
#include "SDL/include/SDL.h"

class j1FlyingEnemy : public j1Entity
{
public:
	j1FlyingEnemy();
	~j1FlyingEnemy() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	void calculate_path();
	void blit_path();
	void check_path_toMove();

private:	
	bool falling = false;	

	float x_pos, y_pos;

private:

	FEnemy_States state;
	bool moving_right = false;
	bool moving_left = false;
	bool falling = false;
	bool isground = false;
	bool set_path = true;
	bool set_timer = false;

	float deltaTime;

	float velocity;
	float fall_vel;

	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* debug_tex = nullptr;

	int path_num = 0;
	int tick1 = 0, tick2 = 0;
};

#endif // !_FLYINGENEMY_
