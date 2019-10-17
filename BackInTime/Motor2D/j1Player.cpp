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

j1Player::j1Player() : j1Module()
{
	name.create("player");
	
	float speed = 0.2f;
	//IDLE
	idle.PushBack({ 0,0,17,27 }, speed);
	idle.PushBack({ 32,0,17,27 }, speed);
	idle.PushBack({ 64,0,19,27 }, speed);
	idle.PushBack({ 96,0,21,27 }, speed);

	//WALK
	speed = 0.2f;
	walk.PushBack({ 0,27,17,25 }, speed);
	walk.PushBack({ 32,27,17,26 }, speed);
	walk.PushBack({ 64,27,17,27 }, speed);
	walk.PushBack({ 95,29,18,26 }, speed);
	walk.PushBack({ 128,28,17,26 }, speed);
	walk.PushBack({ 160,27,17,27 }, speed);	

	//JUMP
	speed = 0.2f;
	jump.PushBack({ 0,58,17,26 }, speed);
	jump.PushBack({ 30,60,20,24 }, speed);
	jump.PushBack({ 61,60,22,24 }, speed);
	jump.PushBack({ 96,55,17,29 }, speed);
	jump.PushBack({ 127,54,18,27 }, speed);
	jump.PushBack({ 158,56,20,27 }, speed);
	jump.PushBack({ 189,60,22,24 }, speed);
	jump.PushBack({ 223,58,18,26 }, speed);

	//RUN
	speed = 0.15f;
	run.PushBack({ 0,141,19,25 }, speed);
	run.PushBack({ 33,141,18,25 }, speed);
	run.PushBack({ 64,141,19,26 }, speed);
	run.PushBack({ 96,141,19,25 }, speed);
	run.PushBack({ 129,141,18,25 }, speed);
	run.PushBack({ 161,141,18,28 }, speed);
}

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	current_animation = &idle;
	collider = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start
	return ret;
}
bool j1Player::Start(){

	spritesheet_pj = App->tex->Load("character/spritesheet_pj.png");	

	return true;
}

bool j1Player::PreUpdate() 
{
	collider->SetPos(x, y);
	return true;
}

bool j1Player::Update(float dt) {

	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		x += velocity;
		current_animation = &walk;
		moving_right = true;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			x += run_velocity;
			current_animation = &run;
		}
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) != true) && moving_right == true) {
		velocity = velocity - decrease_vel;
		x += velocity;
		if (velocity <= 0) {
			moving_right = false;
			velocity = 2.0f;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		x -= velocity;
		current_animation = &walk;
		moving_left = true;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			x -= run_velocity;
			current_animation = &run;
		}
	}
	else if ((App->input->GetKey(SDL_SCANCODE_A) != true) && moving_left == true) { //not working
		velocity = velocity - decrease_vel;
		x -= velocity;
		if (velocity <= 0) {
			moving_left = false;
			velocity = 2.0f;
		}
	}
	y += gravity;


	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(spritesheet_pj, x, y, &r);

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	switch (c2->type)
	{
	case COLLIDER_WALL:
		//position = lastPosition;
		velocity = 0;
		if(y < c2->rect.y)
		{
			//state = IDLE;
			//fall.Reset();
			gravity = false;
		}
		
		break;
	default:
		break;
	}
}

