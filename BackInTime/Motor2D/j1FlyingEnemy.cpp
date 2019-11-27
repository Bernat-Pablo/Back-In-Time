#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1RectSprites.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Fade.h"
#include "Brofiler/Brofiler.h"
#include "j1FlyingEnemy.h"

j1FlyingEnemy::j1FlyingEnemy() : j1Module()
{
	name.create("flyingEnemy");

	float speed = 0.55f;
	
	//FLY
	fly.PushBack({ 2,73,30,35 },speed); //TODO: UGLY, TO FIX Animation Pushback
	fly.PushBack({ 36,73,31,36 }, speed);
	fly.PushBack({ 73,72,27,39 }, speed);
	fly.PushBack({ 107,72,27,39 }, speed);
	fly.PushBack({ 139,75,32,31 }, speed);
	fly.PushBack({ 174,76,32,28 }, speed);
	fly.PushBack({ 208,76,33,28 }, speed);
	fly.PushBack({ 243,75,33,31 }, speed);

	//GROUND
	speed = 0.2f;
	ground.PushBack({ 1,1,32,30 }, speed);
	ground.PushBack({ 35,1,32,30 }, speed);
	ground.PushBack({ 70,1,32,30 }, speed);
	ground.PushBack({ 104,0,32,30 }, speed);

	//HIT
	speed = 0.3f; //TODO: UGLY, TO FIX Animation Pushback
	hit.PushBack({ 0,35,33,35 }, speed);	
	hit.PushBack({ 35,35,33,35 }, speed);
	hit.PushBack({ 70,35,33,35 }, speed);
	hit.PushBack({ 104,35,33,35 }, speed);
	hit.PushBack({ 139,35,33,35 }, speed);
	
	//FALL
	speed = 0.3f;
	fall.PushBack({ 2,111,30,39 }, speed);
	fall.PushBack({ 36,111,30,39 }, speed);
	fall.PushBack({ 71,111,30,39 }, speed);
	fall.PushBack({ 106,111,30,39 }, speed);

	//SETTING VARIABLES
	velocity = 2.0f;
	fall_vel = 3.5f;

}

bool j1FlyingEnemy::Awake(pugi::xml_node&)
{
	x_pos = 752.0;
	y_pos = 30.0;
	return true;
}

bool j1FlyingEnemy::Start()
{
	spritesheet = App->tex->Load("character/bird_spritesheet.png");
	state = FLY;

	return true;
}

bool j1FlyingEnemy::PreUpdate()
{
	//STATE MACHINE
	switch (state)
	{
	case FLY:
		if (moving_right == true) {
			state = FLY_FORWARD;
		}
		else if (moving_left == true) {
			state = FLY_BACKWARD;
		}
		else if (falling == true) {
			state = FALL;
		}
		break;
	case FLY_FORWARD:
		if (moving_right == false) {
			state = FLY;
		}
		break;
	case FLY_BACKWARD:
		if (moving_left == false) {
			state = FLY;
		}
		break;
	case FALL:
		if (falling == false) {
			state = IN_GROUND;
		}
		break;
	case IN_GROUND:

		break;
	}
	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	//STATE MACHINE APPLYING MOVEMENT
	switch (state)
	{
	case FLY:
		current_animation = &fly;

		break;
	case FLY_FORWARD:
		current_animation = &fly;
		x_pos += velocity;

		break;
	case FLY_BACKWARD:
		current_animation = &fly;
		x_pos -= velocity;

		break;
	case FALL:
		current_animation = &fall;
		y_pos += fall_vel;

		break;
	case IN_GROUND:
		current_animation = &ground;

		break;
	default:
		break;
	}

	//BLIT
	App->render->Blit(spritesheet, x_pos, y_pos, &current_animation->GetCurrentFrame());
	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(spritesheet);
	return true;
}

void j1FlyingEnemy::calculate_path()
{

}

void j1FlyingEnemy::blit_path()
{

}
