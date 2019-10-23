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
	jump.loop = false;

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
	int camera_collider_h = 80;
	int camera_collider_w = 5;
	gravity = true;
	position.x = initial_x;
	position.y = initial_y;
	collider_player = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start
	camera_toRight = App->collision->AddCollider({ position.x + 50,position.y - 30,camera_collider_w,camera_collider_h }, COLLIDER_CAMERA, (j1Module*)App->player); 
	return ret;
}
bool j1Player::Start(){

	spritesheet_pj = App->tex->Load("character/spritesheet_pj.png");	

	return true;
}

bool j1Player::PreUpdate() 
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode == true)
		{
			godMode = false;
			position.y -= gravity;
		}			
		else if (godMode == false)
			godMode = true;
	}
	
	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	player_input.pressing_lshift = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT;
	
	switch(state)
	{
	case IDLE:
		if (player_input.pressing_space && in_air==false ) //fix
		{
			jump_vel = 6.5f; //magic numbers. change
			state = JUMP;
		}
		else if (player_input.pressing_D)
		{
			state = WALK_FORWARD;

		}
		else if (player_input.pressing_A)
		{
			state = WALK_BACKWARD;
		}
		break;
	case WALK_FORWARD:
		jump_vel = 6.5f; //magic numbers. change
		if (!player_input.pressing_D && moving_right == true)
		{
			state = DASH_FORWARD;
		}
		if (player_input.pressing_space)
		{
			state = JUMP_FORWARD;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_FORWARD;
		}
		break;
	case WALK_BACKWARD:
		jump_vel = 6.5f; //magic numbers. change
		if (!player_input.pressing_A && moving_left == true)
		{
			state = DASH_BACKWARD;
		}
		if (player_input.pressing_space)
		{
			state = JUMP_BACKWARD;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_BACKWARD;
		}
		break;
	case RUN_FORWARD:

		jump_vel = 6.5f; //magic numbers. change
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_D)
			{
				state = WALK_FORWARD;
			}
			else
			{
				state = DASH_FORWARD;
			}
		}
		else if (player_input.pressing_A && player_input.pressing_lshift) {
			state = RUN_BACKWARD;
		}
		break;
	case RUN_BACKWARD:

		jump_vel = 6.5f; //magic numbers. change
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_A)
			{
				state = WALK_BACKWARD;
			}
			else
			{
				state = DASH_BACKWARD;
			}
		}
		else if (player_input.pressing_D && player_input.pressing_lshift) {
			state = RUN_FORWARD;
		}
		break;
	case JUMP:
		if (player_input.pressing_D) {
			state = JUMP_FORWARD;
		}
		else if (player_input.pressing_A) {
			state = JUMP_BACKWARD;
		}
		else if (current_animation->Finished())
		{
			state = IDLE;
			jump.Reset();
		}
		break;
	case JUMP_FORWARD:
		if (!player_input.pressing_D) {
			//state = JUMP;
		}
		break;
	case JUMP_BACKWARD:
		if (!player_input.pressing_A) {
			//state = JUMP;
		}
		break;
	case DASH_FORWARD:
		if (player_input.pressing_A) {
			state = WALK_BACKWARD;
			velocity = 2.0f;
			if (player_input.pressing_lshift) {
				state = RUN_BACKWARD;
			}
		}
		break;
	case DASH_BACKWARD:
		if (player_input.pressing_D) {
			state = WALK_FORWARD;
			velocity = 2.0f;
			if (player_input.pressing_lshift) {
				state = RUN_FORWARD;
			}
		}
		break;
	}

	//Change player collider position
	collider_player->SetPos(position.x, position.y);

	lastPosition = position;

	return true;
}

bool j1Player::Update(float dt) 
{
	switch(state)
	{
		case IDLE:
			current_animation = &idle;
			break;
		case WALK_FORWARD:
			current_animation = &walk;

			position.x += velocity;

			moving_right = true;
			break;
		case WALK_BACKWARD:
			current_animation = &walk;

			position.x -= velocity;

			moving_left = true;
			break;
		case RUN_FORWARD:
			current_animation = &run;
			
			position.x += run_velocity;
			break;
		case RUN_BACKWARD:
			current_animation = &run;
			
			position.x -= run_velocity;
			break;
		case JUMP:
			current_animation = &jump;
			in_air = true;
			if (jump_vel >= 0) {
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
			}
			else {
				

			}
			break;
		case JUMP_FORWARD:
			current_animation = &jump;
			in_air = true;
			if (jump_vel >= 0) {
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
				position.x += velocity;
			}
			else state = IDLE;
			break;
		case JUMP_BACKWARD:
			current_animation = &jump;
			in_air = true;
			if (jump_vel >= 0) {
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
				position.x -= velocity;
			}
			else state = IDLE;
			break;
		case DASH_FORWARD:
			current_animation = &walk;

			velocity -= decrease_vel;
			
			position.x += velocity;

			if (velocity <= 0) {
				moving_right = false;
				velocity = 2.0f;
				state = IDLE;
			}
			break;
		case DASH_BACKWARD:
			current_animation = &walk;

			velocity -= decrease_vel;
			position.x -= velocity;
			if (velocity <= 0)
			{
				moving_left = false;
				velocity = 2.0f;
				state = IDLE;
			}
			break;
	}
	if(godMode == false)
	{
		fall_velocity += gravity;
		position.y += fall_velocity;
	}		
	else if(godMode == true)
	{
		if (player_input.pressing_W)
			position.y -= velocity;
		if (player_input.pressing_S)
			position.y += velocity;
	}

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(spritesheet_pj, position.x, position.y, &r);


	return true;
}

bool j1Player::CleanUp() {
	collider_player = nullptr;
	spritesheet_pj = nullptr;

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if(godMode == false)
	{
		switch (c2->type)
		{
		case COLLIDER_WALL:
			position = lastPosition;
			
			if (position.y < c2->rect.y ) //Player is above the ground
			{
				gravity = 0;
				position.y -= fall_velocity;
			}
			if (position.x < c2->rect.x) //Player is at the left of a wall
			{
				if(position.y < c2->rect.y) //
				{
					state = IDLE; //We avoid the dash
					position.x -= velocity;
					position.y -= fall_velocity;
				}				
			}
			if(position.x > c2->rect.x + c2->rect.w - 10) //Player is at the right of a wall
			{
				state = IDLE; //We avoid the dash
				position.x += velocity;
				position.y -= fall_velocity;
			}

			break;
		default:

			break;
		}
	}	
}

