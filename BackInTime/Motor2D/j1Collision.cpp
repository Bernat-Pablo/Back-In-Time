#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"

#include "j1Textures.h"

#include "j1Render.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Character.h"
#include "j1Wot.h"


//Constructor
j1Collision::j1Collision()
{
	debug = false;
	//colliders[COLLIDER_PLAYER][COLLIDER_TERRAIN] = true;
	//colliders[COLLIDER_PLAYER][COLLIDER_WALL] = true;
}

//Destructor
j1Collision::~j1Collision()
{

}
void j1Collision::ActivateDebug()
{
	if (debug == true)
		debug = false;
	else if (debug == false)
		debug = true;	
}

bool j1Collision::isDebugActive()
{
	//Debug is nullptr -- 
	/*if (debug == true)
		return true;
	else
		return false;*/

	//Temporary code. We have to use if(debug == true)
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_REPEAT)
		return true;
	else
		return false;
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