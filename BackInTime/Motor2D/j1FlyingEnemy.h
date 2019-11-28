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
	bool set_path = true;
	bool set_timer = false;

	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* debug_tex = nullptr;

	int path_num = 0;
	int tick1 = 0, tick2 = 0;
};

#endif // !_FLYINGENEMY_
