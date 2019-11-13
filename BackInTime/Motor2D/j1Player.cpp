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

j1Player::j1Player() : j1Module()
{
	name.create("player");
	
	float speed = 0.1f;
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

	folder.create(config.child("folder").child_value());

	//Set initial data of the player
	gravity = config.child("gravity").attribute("value").as_float();
	run_velocity = config.child("run_velocity").attribute("value").as_float();
	velocity = config.child("velocity").attribute("value").as_float();
	fall_velocity = config.child("fall_velocity").attribute("value").as_float();
	jump_vel = config.child("jump_vel").attribute("value").as_float();
	decrease_vel = config.child("decrease_vel").attribute("value").as_float();
	lives = config.child("lives").attribute("value").as_int();
	//spritesheet_source = config.child("spritesheet").attribute("source").as_string();

	doc.load_file("config.xml");
	node = doc.child("config");

	return ret;
}
bool j1Player::Start(){		
	//init clocks
	tick2 = SDL_GetTicks();
	tick4 = SDL_GetTicks();
	spritesheet_pj = App->tex->Load("character/spritesheet_pj.png");
	spritesheet_casper = App->tex->Load("character/spritesheet_casper.png");
	spritesheet_bars = App->tex->Load("character/spritesheet_bars.png");

	pugi::xml_node config_local = App->GetConfig();
	if (App->scene->choose_lv == 1) //We are on map1
	{
		position.x = config_local.child("player").child("initialPosition").child("map1").attribute("x").as_int();
		position.y = config_local.child("player").child("initialPosition").child("map1").attribute("y").as_int();
	}
	else if (App->scene->choose_lv == 2) //We are on map2
	{
		position.x = config_local.child("player").child("initialPosition").child("map2").attribute("x").as_int();
		position.y = config_local.child("player").child("initialPosition").child("map2").attribute("y").as_int();
	}
	//initial state
	state = IDLE;
	current_animation = &idle;

	collider_player = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, "player", (j1Module*)App->player); //a collider to start
	//set colliders to move the camera
	camera_toRight = App->collision->AddCollider({ position.x + 70,position.y - 100,20,140 }, COLLIDER_CAMERA, "right", (j1Module*)App->player);
	camera_toLeft = App->collision->AddCollider({ position.x - 50,position.y - 100,20,140 }, COLLIDER_CAMERA, "left", (j1Module*)App->player);
	camera_toUp = App->collision->AddCollider({ position.x - 50,position.y - 100,140,20 }, COLLIDER_CAMERA, "up", (j1Module*)App->player);
	camera_toDown = App->collision->AddCollider({ position.x - 50,position.y + 20,140,40 }, COLLIDER_CAMERA, "down", (j1Module*)App->player);

	App->audio->LoadFx("audio/fx/jump.wav");

	//bar ability
	bar_0 = { 268,25,62,12 };
	bar_1 = { 268,41,62,12 };
	bar_2 = { 268,57,62,12 };
	bar_3 = { 268,73,62,12 };
	bar_4 = { 268,89,62,12 };

	initial_pos = node.child("player").child("initialPosition").child("map1").attribute("x").as_int() + 70; //look to camera_toright
	screen_size = node.child("window").child("resolution").attribute("scale").as_int();

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

	//check inputs
	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
	player_input.pressing_lshift = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT;
	
	//states machine
	switch (state)
	{
	case IDLE:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_space && in_air==false) //fix
		{
			restart_variables(-1, 1);
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
		restart_variables(-1, 1);

		if (!player_input.pressing_D && moving_right == true)		
			state = DASH_FORWARD;
		
		if (!player_input.pressing_D && moving_right == false)		
			state = IDLE;
		
		if (player_input.pressing_space && in_air == false)		
			state = JUMP_FORWARD;
		
		if (player_input.pressing_lshift)		
			state = RUN_FORWARD;
		
		if (player_input.pressing_A)
		{
			if (collider_at_right)
				position.x -= velocity;

			state = WALK_BACKWARD;
		}
		
		looking_right = true;

		break;
	case WALK_BACKWARD:
		restart_variables(-1, 1);
		if (!player_input.pressing_A && moving_left == true)
			state = DASH_BACKWARD;
		
		if (!player_input.pressing_A && moving_left == false)		
			state = IDLE;
		
		if (player_input.pressing_space && in_air == false)		
			state = JUMP_BACKWARD;
		
		if (player_input.pressing_lshift)		
			state = RUN_BACKWARD;

		looking_right = false;

		break;
	case RUN_FORWARD:
		restart_variables(-1, 1);
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_D)
				state = WALK_FORWARD;			
			else			
				state = DASH_FORWARD;			
		}
		else if (player_input.pressing_A && player_input.pressing_lshift) 
			state = RUN_BACKWARD;	

		if (player_input.pressing_space)
			state = JUMP_FORWARD;

		looking_right = true;

		break;
	case RUN_BACKWARD:
		restart_variables(-1, 1);
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_A)			
				state = WALK_BACKWARD;			
			else			
				state = DASH_BACKWARD;			
		}
		else if (player_input.pressing_D && player_input.pressing_lshift) 
			state = RUN_FORWARD;
		
		if (player_input.pressing_space)
			state = JUMP_BACKWARD;

		looking_right = false;

		break;
	case JUMP:
		if (player_input.pressing_D) 
			state = JUMP_FORWARD;		
		if (player_input.pressing_A) 
			state = JUMP_BACKWARD;
		
		break;
	case JUMP_FORWARD:
		moving_right = true;
		moving_left = false;
		if (!player_input.pressing_D) 
			state = JUMP;		
		break;
	case JUMP_BACKWARD:
		moving_right = false;
		moving_left = true;
		if (!player_input.pressing_A) 
			state = JUMP;		

		looking_right = false;

		break;
	case DASH_FORWARD:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_A) {
			state = WALK_BACKWARD;
			restart_variables(1, -1);
			if (player_input.pressing_lshift)
				state = RUN_BACKWARD;			
		}
		else if (!player_input.pressing_A)
			state = IDLE;
		break;
	case DASH_BACKWARD:
		moving_right = false;
		moving_left = true;
		if (player_input.pressing_D) {
			state = WALK_FORWARD;
			restart_variables(1, -1);
			if (player_input.pressing_lshift)
				state = RUN_FORWARD;			
		}else if (!player_input.pressing_D)
			state = IDLE;
		break;
	}

	//Change player collider position
	collider_player->SetPos(position.x, position.y);

	return true;
}

bool j1Player::Update(float dt) 
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && in_air == false)	App->audio->PlayFx(1, 0); //sound of jumping before update
	
	
	//can we do the main ability?
	checkAbility();

	//aplying the forces depending of the state
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
				moving_right = true;
				moving_left = false;
				position.x += run_velocity;
			}
			else if (collider_at_right == true)
			{
				moving_left = false;
				moving_right = false;
			}
			break;
		case RUN_BACKWARD:
			current_animation = &run;
			if (collider_at_left == false)
			{
				moving_left = true;
				moving_right = false;
				position.x -= run_velocity;
			}else if(collider_at_left == true)
			{
				moving_left = false;
				moving_right = false;
			}
			break;
		case JUMP:
			if (jump_vel > 0) {
				in_air = true;
				current_animation = &jump_up;
				jump_vel -= fall_velocity;
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
			if (jump_vel >= 0) {
				current_animation = &jump_up;
				in_air = true;
				jump_vel -= fall_velocity;
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
			if (jump_vel >= 0) {
				current_animation = &jump_up;
				in_air = true;
				jump_vel -= fall_velocity;
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

			//velocity -= decrease_vel; BUG WITH VELOCITY pendent to solve
			
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

			//velocity -= decrease_vel; BUG WITH VELOCITY pendent to solve

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
		
		in_air = checkInAir();

		if(in_air == true)
		{
			fall_velocity += gravity;
			position.y += fall_velocity;
			LOG("IN AIR");
		}else if(in_air == false)
		{
			LOG("NOT IN AIR");
		}			
	}		
	else if(godMode == true)
	{
		in_air = false;
		if (player_input.pressing_W)
			position.y -= velocity;
		if (player_input.pressing_S)
			position.y += velocity;
	}

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(spritesheet_casper, old_position[0].x, old_position[0].y, &idle.GetCurrentFrame());

	if(!looking_right)
		App->render->Blit(spritesheet_pj, position.x, position.y, &r,1,2); //looking at left
	else
		App->render->Blit(spritesheet_pj, position.x, position.y, &r); //looking at right

	//print shadow player position


	if (player_input.pressing_A == true || player_input.pressing_D == true) //watching if the pj is walkubg
		walking = true;
	else walking = false;

	if (walking == true)	App->audio->PlayFx(2, 1); //sound of walking active

	//do ability
	if (ability_able == true && App->input->GetKey(SDL_SCANCODE_RETURN)==KEY_DOWN) {
		useAbility();
	}	


	return true;
}

bool j1Player::CleanUp() {
	LOG("Unloading player\n");
	collider_player = nullptr;
	spritesheet_pj = nullptr;
	spritesheet_casper = nullptr;
	spritesheet_bars = nullptr;
	current_animation = nullptr;

	//Unload colliders for the camera
	camera_toRight = nullptr;
	camera_toLeft = nullptr;
	camera_toUp = nullptr;
	camera_toDown = nullptr;

	//Unload audio	
	App->audio->unLoadFx("audio/fx/jump.wav");

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if(godMode == false)
	{
		switch (c2->type)
		{
		
		case COLLIDER_WALL:

			if (position.x + collider_player->rect.w < c2->rect.x + 20) //Player is at the left of a wall
			{
				if (position.y + 0.7f * collider_player->rect.h > c2->rect.y) //There is a wall
					collider_at_right = true;
				else
					collider_at_right = false;
			}
			else
				collider_at_right = false;

			if (position.x < c2->rect.x + c2->rect.w) //Player is at the right of a wall
			{
				if(state == WALK_BACKWARD || state == RUN_BACKWARD || state == JUMP_BACKWARD || state == IDLE || state == DASH_BACKWARD)
				{
					if (position.y + 0.7f * collider_player->rect.h > c2->rect.y) //There is a wall
					{
						collider_at_left = true;
					}
					else
						collider_at_left = false;
				}				
			}
			else
				collider_at_left = false;
			break;
		case COLLIDER_DIE:
			//Player goes to initial position
			state = IDLE;
			iterator = 0;
			App->fade->FadeToBlack(App->scene, App->scene);
			
			break;
		case COLLIDER_DOOR:			
			//Change scene from 1 to 2
			if(c2->name == "door1") //We touch the first door, so we go to level 2
				App->scene->choose_lv = 2;
			else if (c2->name == "door2") 
				App->scene->choose_lv = 1;

			iterator = 0;
			App->fade->FadeToBlack(App->scene, App->scene);
			break;	
		}
	}

	//It's out of the if(godMode == false) 
	//because even if it's at godMode, we want the camera to follow the player
	if(c2->type == COLLIDER_CAMERA)
	{
		float localVelocity = 0;
		//We adjust camera velocity to player velocity depending his state
		if (state == WALK_FORWARD || state == WALK_BACKWARD)
			localVelocity = velocity;
		else if (state == RUN_FORWARD || state == RUN_BACKWARD)
			localVelocity = run_velocity;
		else
			localVelocity = velocity;
				
		if (c2->name == "right") //Collision with camera_toRight
		{
			//Update the position of the camera colliders
			MoveCameraColliders("x", localVelocity);

		}
		if (c2->name == "left") //Collision with camera_toLeft
		{
			//Update the position of the camera colliders
			MoveCameraColliders("x", -localVelocity);			
		}
		if (c2->name == "up") //Collision with camera_toUp
		{
			//Update the position of the camera colliders			
			MoveCameraColliders("y", -localVelocity);
		}
		if (c2->name == "down") //Collision with camera_toDown
		{			
			//Update the position of the camera colliders				
			MoveCameraColliders("y", fall_velocity);
			
		}
	}		
}

bool j1Player::Save(pugi::xml_node& data) const {
	
	//Player position
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	//Current level
	data.append_child("level").append_attribute("value") = App->scene->choose_lv;

	//Save colliders for the camera
	data.append_child("camera_toRight").append_attribute("x") = camera_toRight->rect.x;
	data.child("camera_toRight").append_attribute("y") = camera_toRight->rect.y;
	data.append_child("camera_toLeft").append_attribute("x") = camera_toLeft->rect.x;
	data.child("camera_toLeft").append_attribute("y") = camera_toLeft->rect.y;
	data.append_child("camera_toUp").append_attribute("x") = camera_toUp->rect.x;
	data.child("camera_toUp").append_attribute("y") = camera_toUp->rect.y;
	data.append_child("camera_toDown").append_attribute("x") = camera_toDown->rect.x;
	data.child("camera_toDown").append_attribute("y") = camera_toDown->rect.y;
	
	//Save camera position
	data.append_child("camera").append_attribute("x") = App->render->camera.x;
	data.child("camera").append_attribute("y") = App->render->camera.y;

	//Extra data
	data.append_child("lives").append_attribute("value") = lives;

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{	
	if (App->scene->choose_lv != data.child("level").attribute("value").as_int())
	{
		App->scene->choose_lv = data.child("level").attribute("value").as_int();
		App->fade->FadeToBlack(App->scene, App->scene);
	}

	//Load player position
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	//Load camera positions
	camera_toRight->rect.x = data.child("camera_toRight").attribute("x").as_int();
	camera_toRight->rect.y = data.child("camera_toRight").attribute("y").as_int();
	camera_toLeft->rect.x = data.child("camera_toLeft").attribute("x").as_int();
	camera_toLeft->rect.y = data.child("camera_toLeft").attribute("y").as_int();
	camera_toUp->rect.x = data.child("camera_toUp").attribute("x").as_int();
	camera_toUp->rect.y = data.child("camera_toUp").attribute("y").as_int();
	camera_toDown->rect.x = data.child("camera_toDown").attribute("x").as_int();
	camera_toDown->rect.y = data.child("camera_toDown").attribute("y").as_int();

	//Load camera position
	App->render->camera.x = data.child("camera").attribute("x").as_int();
	App->render->camera.y = data.child("camera").attribute("y").as_int();

	//Load extra data
	lives = data.child("lives").attribute("value").as_int();

	

	return true;
}

void j1Player::checkAbility() {
	if (tick1 - tick2 >= 1) { // here we save each 0.05 sec the position.
		if (iterator <= 29) {
			old_position[iterator].x = App->player->position.x;
			old_position[iterator].y = App->player->position.y;
			iterator++;
		}
		else {
			for (int i = 0; i <= 28; i++) { //if its full, we move the array
				old_position[i].x = old_position[i + 1].x;
				old_position[i].y = old_position[i + 1].y;
			}
			old_position[29].x = App->player->position.x; //what we pretend to do, is save the position of 1.5 sec before in time
			old_position[29].y = App->player->position.y;
		}
		tick2 = SDL_GetTicks();
	}
	tick1 = SDL_GetTicks();

	//blit bar to fix
	if (tick3 - tick4 >= 800) {
		if (tick3 - tick4 >= 1600) {
			if (tick3 - tick4 >= 2400) {
				if (tick3 - tick4 >= 3200) {
					if (tick3 - tick4 >= 4000) {
						App->render->Blit(spritesheet_bars, position.x - initial_pos, -70, &bar_0);
					}
					else
						App->render->Blit(spritesheet_bars, position.x - initial_pos, -70, &bar_1);
				}
				else
					App->render->Blit(spritesheet_bars, position.x - initial_pos, -70, &bar_2);
			}
			else 
				App->render->Blit(spritesheet_bars, position.x - initial_pos, -70, &bar_3);
		}
		else 
			App->render->Blit(spritesheet_bars, position.x - initial_pos, -70, &bar_4);
	}


	//we just can use the ab each 4 seconds
	if (tick3 - tick4 >= 4000) {
		ability_able = true;
		tick4 = SDL_GetTicks();
	}
	tick3 = SDL_GetTicks();

	
}

void j1Player::useAbility() {

	position.x = old_position[0].x; //we pass the first position of the array to the actual position
	position.y = old_position[0].y;
	 // we move all the camera colliders
	camera_toRight->SetPos(position.x + 70, position.y - 100); //this +70 is added to initial pos too.
	camera_toLeft->SetPos(position.x - 50, position.y - 100);
	camera_toUp->SetPos(position.x - 50, position.y - 100);
	camera_toDown->SetPos(position.x - 50, position.y + 20);
	
	position_when_ability = -position.x * screen_size + initial_pos;

	//remove magin numbers
	App->render->camera.x = -position.x*2+ 150; //we move the player position

	for (int i = 0; i <= 29; i++) {
		old_position[i].y = position.y;
		old_position[i].x = position.x;
	}

	ability_able = false;
}

void j1Player::MoveCameraColliders(p2SString direction, float speed)
{
	//How to call this function to move camera colliders

	//Move Left: MoveCameraColliders("x", -5.0f);
	//Move Right: MoveCameraColliders("x", 5.0f);
	//Move Up: MoveCameraColliders("y", -5.0f);
	//Move Down: MoveCameraColliders("y", 5.0f);

	if(direction == "x") //Move right or left
	{
		if(speed < 0 ) //moving to left
			if(App->render->camera.x <= -1) //Camera is inside the map
				App->render->camera.x -= 2*speed;
		if (speed > 0) //moving to right
			if ((-App->render->camera.x + App->render->camera.w) <= App->map->data.width*32) //Camera is inside the map
				App->render->camera.x -= 2 * speed;

		camera_toRight->rect.x += speed;
		camera_toLeft->rect.x += speed;
		camera_toUp->rect.x += speed;
		camera_toDown->rect.x += speed;
	}else if(direction == "y") //Move up or down
	{
		if (speed < 0) //moving up
				App->render->camera.y -= 2 * speed;
		if (speed > 0) //moving down
			if ((-App->render->camera.y + App->render->camera.h) < App->map->data.height * 32 -16) //Camera is inside the map
				App->render->camera.y -= 2 * speed;

		camera_toRight->rect.y += speed;
		camera_toLeft->rect.y += speed;
		camera_toUp->rect.y += speed;
		camera_toDown->rect.y += speed;
	}
}

bool j1Player::checkInAir() //Checks if player is in_air or if it's grounded
{
	// Calculate collisions
	Collider* c2;

	// avoid checking collisions already checked
	for (uint k = 0; k < MAX_COLLIDERS; ++k)
	{
		// skip empty colliders
		if (App->collision->colliders[k] == nullptr)
			continue;

		c2 = App->collision->colliders[k];

		if(c2->type == COLLIDER_WALL) //We only want to check if player is colliding with a wall
		{
			if (collider_player->CheckCollision(c2->rect) == true) //There is collision between the player and a wall
			{
				if (position.y < c2->rect.y) //Player is on the ground
				{
					if (position.x + 0.8 * collider_player->rect.w > c2->rect.x)
					{
						if (position.x < c2->rect.x + c2->rect.w - 0.2 * collider_player->rect.w)
						{
							fall_velocity = 1;
							return false;							
						}
					}
				}
			}
		}	
	}
	
	return true; //We didn't found a collision with a wall, so in_air = true. Player is not grounded
}

void j1Player::restart_variables(int vel, int vel_jump) {
	if (vel != -1) {
		velocity = node.child("player").child("velocity").attribute("value").as_float();
	}
	if (vel_jump != -1) {
		jump_vel= node.child("player").child("jump_vel").attribute("value").as_float();
	}
}
