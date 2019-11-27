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

enum Player_States {
	IDLE,
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

	iPoint position; //Use position.x and position.y

private:

	Animation idle;
	Animation fly;
	Animation ground;
	Animation hit;
	Animation fall;
	Animation* current_animation;

};
#endif // !_FLYINGENEMY_
