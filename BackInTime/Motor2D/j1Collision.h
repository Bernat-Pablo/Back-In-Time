#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 250

#include "j1Module.h"
#include "j1App.h"
#include "SDL/include/SDL.h"

enum COLLIDER_TYPE
{
	COLLIDER_PLAYER,

	COLLIDER_WALL,
	COLLIDER_DIE,
	COLLIDER_DOOR,
	COLLIDER_CAMERA,
	COLLIDER_FLYING_ENEMY,
	COLLIDER_GROUND_ENEMY,

	COLLIDER_MAX,
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	p2SString name;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, p2SString name, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback),
		name(name)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	void SetSize(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}
	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:
	j1Collision();
	~j1Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void DebugDraw();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, p2SString name, j1Module* callback = nullptr);
	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
private:

	bool debug = false;
};

#endif // __ModuleCollision_H__