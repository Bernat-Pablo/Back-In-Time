#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"

//Constructor
j1Collision::j1Collision()
{
	
	//colliders[COLLIDER_PLAYER][COLLIDER_TERRAIN] = true;
	//colliders[COLLIDER_PLAYER][COLLIDER_WALL] = true;
}

//Destructor
j1Collision::~j1Collision()
{

}


/*bool ModuleCollision::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		DebugDraw();
}*/

bool j1Collision::CleanUp()
{

	return true;
}