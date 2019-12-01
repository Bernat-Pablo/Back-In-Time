#ifndef _GROUNDENEMY_
#define _GROUNDENEMY_

#include "j1Entity.h"
#include "SDL/include/SDL.h"

class j1GroundEnemy : public j1Entity
{
public:
	j1GroundEnemy();
	~j1GroundEnemy() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	void calculate_path();
	void blit_path();
	void check_path_toMove();

	bool checkInAir();

private:
	bool set_path = true;
	bool set_timer = false;

	SDL_Texture* debug_tex = nullptr;

	int path_num = 0;
	int tick1 = 0, tick2 = 0;

	int speed;

	bool stun = false;
	bool reset = true;
	bool ready = true;
	bool falling = true;

	iPoint objective;

};

#endif // !_FLYINGENEMY_
