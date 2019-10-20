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
	position.x = 32;
	position.y = 350;
	can_move_left = can_move_right = true;
	collider_player = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start
	return ret;
}
bool j1Player::Start(){

	spritesheet_pj = App->tex->Load("character/spritesheet_pj.png");	

	return true;
}

bool j1Player::PreUpdate() 
{
	can_move_left = true;
	can_move_right = true;

	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	player_input.pressing_lshift = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT;
	
	switch(state)
	{
	case IDLE:
		if (player_input.pressing_W)
		{
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
		else if (player_input.pressing_S)
		{

		}
		break;
	case WALK_FORWARD:
		if (!player_input.pressing_D && moving_right == true)
		{
			state = DASH_FORWARD;
		}
		if (player_input.pressing_space)
		{
			state = JUMP;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_FORWARD;
		}
		break;
	case WALK_BACKWARD:
		if (!player_input.pressing_A && moving_left == true)
		{
			state = DASH_BACKWARD;
		}
		if (player_input.pressing_space)
		{
			state = JUMP;
		}
		if (player_input.pressing_lshift)
		{
			state = RUN_BACKWARD;
		}
		break;
	case RUN_FORWARD:
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
		break;
	case RUN_BACKWARD:
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
		break;
	case JUMP:
		if (current_animation->Finished())
		{
			state = IDLE;
			jump.Reset();
		}
		break;
	case JUMP_FORWARD:

		break;
	case JUMP_BACKWARD:

		break;
	case DASH_FORWARD:
		//Written on Update()
		//It changes to IDLE if velocity <= 0
		break;
	case DASH_BACKWARD:
		//Written on Update()
		//It changes to IDLE if velocity <= 0
		break;
	}

	//Change player collider position
	collider_player->SetPos(position.x, position.y);

	lastPosition = position;

	return true;
}

bool j1Player::Update(float dt) {

	current_animation = &idle;

	switch(state)
	{
		case IDLE:
			current_animation = &idle;
			break;
		case WALK_FORWARD:
			current_animation = &walk;

			if(can_move_right)
				position.x += velocity;

			current_animation = &walk;
			moving_right = true;
			break;
		case WALK_BACKWARD:
			current_animation = &walk;

			if (can_move_left)
				position.x -= velocity;

			current_animation = &walk;
			moving_left = true;
			break;
		case RUN_FORWARD:
			current_animation = &run;
			
			if (can_move_right)
				position.x += run_velocity;
			break;
		case RUN_BACKWARD:
			current_animation = &run;
			
			if (can_move_left)
				position.x -= run_velocity;
			break;
		case JUMP:
			current_animation = &jump;

			if (player_input.pressing_A)
				position.x -= velocity;
			else if (player_input.pressing_D)
				position.x += velocity;
			break;
		case JUMP_FORWARD:
			current_animation = &jump;
			break;
		case JUMP_BACKWARD:
			current_animation = &jump;
			break;
		case DASH_FORWARD:
			current_animation = &walk;

			velocity = velocity - decrease_vel;
			if (can_move_right)
				position.x += velocity;
			if (velocity <= 0) {
				moving_right = false;
				velocity = 2.0f;
				state = IDLE;
			}
			break;
		case DASH_BACKWARD:
			current_animation = &walk;

			//not working
			velocity = velocity - decrease_vel;
			if (can_move_left)
			position.x -= velocity;
			if (velocity <= 0)
			{
				moving_left = false;
				velocity = 2.0f;
				state = IDLE;
			}
			break;
	}
	position.y += gravity;

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

	switch (c2->type)
	{
	case COLLIDER_WALL:
		position = lastPosition;
		velocity = 0;		

		if (position.y < c2->rect.y) //Player is above the ground
		{
			state = IDLE;
			gravity = false;
			position.y -= 2;
			isGrounded = true;
		}
		if(position.x < c2->rect.x) //Player is at the left of a wall
		{
			can_move_right = false;
		}
		if (position.x > c2->rect.x) //Player is at the right of a wall
		{
			can_move_left = false;
		}

		break;
	default:

		break;
	}	
}

