#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 500000

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Log.h"

enum COLLIDER_TYPE
{
	COLLIDER_PLAYER,
	COLLIDER_WALL, //Solid objects

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	COLLIDER_TYPE type;
	bool to_delete = false;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
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

	bool PreUpdate() override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	
private:
	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	
};

#endif // __ModuleCollision_H__