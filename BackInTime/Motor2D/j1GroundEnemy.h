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
	void BlitEverything();

	bool checkInAir();

private:
	bool set_path;
	bool set_timer;

	SDL_Texture* debug_tex;

	int path_num;
	int tick1, tick2;

	int speed;

	bool stun;
	bool reset;
	bool ready;

	iPoint objective;

};

#endif // !_FLYINGENEMY_
