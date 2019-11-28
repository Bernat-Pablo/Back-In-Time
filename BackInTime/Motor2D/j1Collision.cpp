#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

//Constructor
j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
	
	//To collide with the ground and walls
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	
	//Colliders to move the camera
	matrix[COLLIDER_PLAYER][COLLIDER_CAMERA] = true;
	matrix[COLLIDER_CAMERA][COLLIDER_PLAYER] = true;

	//Colliders to die
	matrix[COLLIDER_PLAYER][COLLIDER_DIE] = true;
	matrix[COLLIDER_DIE][COLLIDER_PLAYER] = true;

	//Colliders to change scene
	matrix[COLLIDER_PLAYER][COLLIDER_DOOR] = true;
	matrix[COLLIDER_DOOR][COLLIDER_PLAYER] = true;

	//Flying enemies and player
	matrix[COLLIDER_PLAYER][COLLIDER_FLYING_ENEMY] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_PLAYER] = true;

	//Ground enemis and player
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND_ENEMY] = true;
	matrix[COLLIDER_GROUND_ENEMY][COLLIDER_PLAYER] = true;
}

//Destructor
j1Collision::~j1Collision()
{

}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			//delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

bool j1Collision::PreUpdate()
{
	bool ret = true;
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

			}
			
		}
	}
	BROFILER_CATEGORY("Collision_PreUpdate", Profiler::Color::Bisque);

	return ret;
}

bool j1Collision::Update(float dt)
{
	bool ret = true;	
	DebugDraw();
	BROFILER_CATEGORY("Collision_Update", Profiler::Color::Black);
	return ret;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, p2SString name, j1Module* callback )
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, name, callback);
			if(ret)
				LOG("Collider added successfully! ");

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

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		for (int i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;

			switch (colliders[i]->type)
			{
			case COLLIDER_WALL:
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, 50);
				break;
			case COLLIDER_PLAYER:
				App->render->DrawQuad(colliders[i]->rect, 0, 153, 0, 80);
				break;
			case COLLIDER_DIE:
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, 80);
				break;
			case COLLIDER_FLYING_ENEMY:
				App->render->DrawQuad(colliders[i]->rect, 211, 84, 0, 150);
				break;
			case COLLIDER_GROUND_ENEMY:
				App->render->DrawQuad(colliders[i]->rect, 247, 220, 111, 50);
				break;
			case COLLIDER_DOOR:
				App->render->DrawQuad(colliders[i]->rect, 93, 109, 126, 150);
				break;
			case COLLIDER_CAMERA:
				App->render->DrawQuad(colliders[i]->rect, 20, 109, 126, 150);
				break;
			}
			
		}
	}	
}