#include "j1Collision.h"
#include "j1App.h"

//Constructor
ModuleCollision::ModuleCollision()
{
	colliders[COLLIDER_PLAYER][COLLIDER_TERRAIN] = true;
	colliders[COLLIDER_PLAYER][COLLIDER_WALL] = true;
}

//Destructor
ModuleCollision::~ModuleCollision()
{

}
void ModuleCollision::DebugDraw()
{
	
		
	
}

/*bool ModuleCollision::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN)
		DebugDraw();
}*/

bool ModuleCollision::CleanUp()
{

	return true;
}