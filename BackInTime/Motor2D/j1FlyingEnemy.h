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
	void BlitEverything();
private:
	bool falling;
	bool set_path;
	bool set_timer;
	bool starting_flying;

	SDL_Texture* debug_tex;

	int path_num;
	int tick1, tick2;
};

#endif // !_FLYINGENEMY_
