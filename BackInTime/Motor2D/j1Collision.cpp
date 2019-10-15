#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"

//Constructor
j1Collision::j1Collision()
{	
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
}

//Destructor
j1Collision::~j1Collision()
{

}

bool j1Collision::CleanUp()
{

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}