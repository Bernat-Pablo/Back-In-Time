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

	//JUMP UP
	speed = 0.1f;
	jump_up.PushBack({ 0,58,17,26 }, speed);
	jump_up.PushBack({ 30,60,20,24 }, speed);
	jump_up.PushBack({ 61,60,22,24 }, speed);
	jump_up.PushBack({ 96,55,17,29 }, speed);
	jump_up.loop = true;
	
	//JUMP DOWN
	jump_down.PushBack({ 127,54,18,27 }, speed);
	jump_down.PushBack({ 158,56,20,27 }, speed);
	jump_down.PushBack({ 189,60,22,24 }, speed);
	jump_down.PushBack({ 223,58,18,26 }, speed);
	jump_down.loop = true;

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
	//God Mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode == true)
		{
			godMode = false;
		}			
		else if (godMode == false)
		{
			godMode = true;
			collider_at_left = false;
			collider_at_right = false;
		}			
	}
		
	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	player_input.pressing_lshift = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT;
	
	switch (state)
	{
	case IDLE:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_space && in_air==false) //fix
		{
			jump_vel = 6.5f; //magic numbers. change
			state = JUMP;
			in_air = true;
		}
		else if (player_input.pressing_D)
		{
			if (collider_at_right == false)
			state = WALK_FORWARD;
		}
		else if (player_input.pressing_A)
		{
			if (collider_at_left == false)
			{				
				state = WALK_BACKWARD;
			}
		}
		break;
	case WALK_FORWARD:		
		//Bug warning
		//moving_right and moving_left is changed on Update() depending on colliders
		//If that code is uncommented, player will go through near colliders using the dash, causing a bug
		
		//moving_right = true;
		//moving_left = false;
		jump_vel = 6.5f; //magic numbers. change

		if (!player_input.pressing_D && moving_right == true)
		{
			state = DASH_FORWARD;
		}
		if (!player_input.pressing_D && moving_right == false)
		{
			state = IDLE;
		}
		if (player_input.pressing_space && in_air == false)
		{
			state = JUMP_FORWARD;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_FORWARD;
		}
		if (player_input.pressing_A)
		{
			if (collider_at_right)
				position.x -= velocity;
			state = WALK_BACKWARD;
		}
			
		break;
	case WALK_BACKWARD:
		//Bug warning
		//moving_right and moving_left is changed on Update() depending on colliders
		//If that code is uncommented, player will go through near colliders using the dash, causing a bug

		//moving_right = false; 
		//moving_left = true;
		jump_vel = 6.5f; //magic numbers. change
		if (!player_input.pressing_A && moving_left == true)
		{
			state = DASH_BACKWARD;
		}
		if (!player_input.pressing_A && moving_left == false)
		{
			state = IDLE;
		}
		if (player_input.pressing_space && in_air == false)
		{
			state = JUMP_BACKWARD;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_BACKWARD;
		}
		break;
	case RUN_FORWARD:
		moving_right = true;
		moving_left = false;
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
		moving_right = false;
		moving_left = true;
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
		
		break;
	case JUMP_FORWARD:
		moving_right = true;
		moving_left = false;
		if (!player_input.pressing_D) {
			state = JUMP;
		}
		break;
	case JUMP_BACKWARD:
		moving_right = false;
		moving_left = true;
		if (!player_input.pressing_A) {
			state = JUMP;
		}
		break;
	case DASH_FORWARD:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_A) {
			state = WALK_BACKWARD;
			velocity = 2.0f;
			if (player_input.pressing_lshift) {
				state = RUN_BACKWARD;
			}
		}
		break;
	case DASH_BACKWARD:
		moving_right = false;
		moving_left = true;
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

			if(collider_at_right == false)
			{
				position.x += velocity;
				moving_right = true;				
			}
			else			
				moving_right = false;
			
			break;
		case WALK_BACKWARD:
			current_animation = &walk;

			if (collider_at_left == false || collider_at_right == true)
			{
				position.x -= velocity;
				moving_left = true;
			}
			else
				moving_left = false;
			
			break;
		case RUN_FORWARD:
			current_animation = &run;
			if (collider_at_right == false)
			{
				position.x += run_velocity;
			}
			break;
		case RUN_BACKWARD:
			current_animation = &run;
			if (collider_at_left == false)
			{
				position.x -= run_velocity;
			}
			break;
		case JUMP:
			if (jump_vel > 0) {
				current_animation = &jump_up;
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
			}
			else {
				current_animation = &jump_down;
				if (in_air == false) {
					state = IDLE;
				}
			}
			break;
		case JUMP_FORWARD:
			current_animation = &jump_up;
			if (jump_vel >= 0) {
				in_air = true;
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
			}
			else {
				current_animation = &jump_down;
				if (in_air == false) {
					state = WALK_FORWARD;
					moving_right = true;
				}
			}
			if (collider_at_right == false)
				position.x += velocity;

			break;
		case JUMP_BACKWARD:
			current_animation = &jump_up;
			if (jump_vel >= 0) {
				in_air = true;
				jump_vel -= decrease_vel;
				position.y -= jump_vel;
			}
			else {
				current_animation = &jump_down;
				if (in_air == false) {
					state = WALK_BACKWARD;
					moving_left = true;
				}
			}
			if (collider_at_left == false)
				position.x -= velocity;
			break;
		case DASH_FORWARD:
			current_animation = &walk;

			velocity -= decrease_vel;
			
			if(collider_at_right == false)
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
			if(collider_at_left == false)
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
			if (position.y <
				c2->rect.y ) //Player is above the ground
			{
				gravity = 0;
				position.y -= fall_velocity;
				in_air = false;
				jump_down.Reset();
				jump_up.Reset();
			}
			if (position.x + collider_player->rect.w < c2->rect.x + 20) //Player is at the left of a wall
			{
				if(position.y + 0.7f*collider_player->rect.h > c2->rect.y) //There is a wall
					collider_at_right = true;
				else
					collider_at_right = false;
			}
			else
				collider_at_right = false;
			
			if(position.x < c2->rect.x + c2->rect.w && (state == WALK_BACKWARD || state == RUN_BACKWARD || state == JUMP_BACKWARD)) //Player is at the right of a wall
			{
				if (position.y + 0.7f * collider_player->rect.h > c2->rect.y) //There is a wall
				{
					collider_at_left = true;
				}					
				else
					collider_at_left = false;
			}
			else
				collider_at_left = false;



			break;
		default:

			break;
		}
	}	
}

