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

j1Player::j1Player() : j1Entity(entityTypes::PLAYER)
{
	name.create("player");

	spritesheet_rock = nullptr;
	spritesheet_casper = nullptr;
	spritesheet_bars = nullptr;
	spritesheet_source = nullptr;

	in_air = false;
	looking_right = true;

	godMode = false;
	walking = false; //is he walking?

	ability_able = false;

	rock_timer = 0;
	rock_fall_velocity = 0;

	tick1 = 0, tick2 = 0;
	tick3 = 0, tick4 = 0;

	camera_toRight = nullptr;
	camera_toLeft = nullptr;
	camera_toUp = nullptr;
	camera_toDown = nullptr;

	collider_rock = nullptr;

	livesUpdated = false;

	collected_coins = 0;

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

	throw_rock.PushBack({ 128,218,12,7 }, speed);
	throw_rock.loop = true;
}

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;

	config = App->GetConfig();
	gravity = config.child("entityManager").child("gravity").attribute("value").as_float(); //We set gravity before going into player
	config = config.child("entityManager").child("player");

	//Set initial data of the player	
	run_velocity = config.child("run_velocity").attribute("value").as_float();
	velocity = config.child("velocity").attribute("value").as_float();
	fall_velocity = config.child("fall_velocity").attribute("value").as_float();
	max_fall_velocity = config.child("max_fall_velocity").attribute("value").as_float();
	jump_vel = config.child("jump_vel").attribute("value").as_float();
	decrease_vel = config.child("decrease_vel").attribute("value").as_float();
	lives = config.child("lives").attribute("value").as_int();
	//spritesheet_source = config.child("spritesheet").attribute("source").as_string();

	//Set initial data of the rock
	rockVelocity.x = config.child("rock").child("velocity").attribute("x").as_float();
	rockVelocity.y = config.child("rock").child("velocity").attribute("y").as_float();
	rock_cooldown = config.child("rock").child("cooldown").attribute("value").as_float();
	rock_gravity = config.child("rock").child("gravity").attribute("value").as_float();
	rockPosition.x = -5000;
	rockPosition.y = -5000;

	doc.load_file("config.xml");

	return ret;
}

bool j1Player::Start() {
	//init clocks
	tick2 = SDL_GetTicks();
	tick4 = SDL_GetTicks();

	livesUpdated = false;

	spritesheet_entity = App->tex->Load("character/spritesheet_pj.png");
	spritesheet_casper = App->tex->Load("character/spritesheet_casper.png");
	spritesheet_bars = App->tex->Load("character/spritesheet_bars.png");
	spritesheet_rock = App->tex->Load("character/spritesheet_pj.png");

	pugi::xml_node config_local = App->GetConfig();

	//We get screen_size before moving config_local inside entities
	screen_size = config_local.child("window").child("resolution").attribute("scale").as_int();

	config_local = config_local.child("entityManager").child("player");
	fall_velocity = 0;
	if (App->scene->choose_lv == 1) //We are on map1
	{
		position.x = config_local.child("initialPosition").child("map1").attribute("x").as_int();
		position.y = config_local.child("initialPosition").child("map1").attribute("y").as_int();
	}
	else if (App->scene->choose_lv == 2) //We are on map2
	{
		position.x = config_local.child("initialPosition").child("map2").attribute("x").as_int();
		position.y = config_local.child("initialPosition").child("map2").attribute("y").as_int();
	}
	//initial state
	current_animation = &idle;

	collider_entity = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, "player", (j1Module*)App->player); //a collider to start
	collider_rock = App->collision->AddCollider(throw_rock.GetCurrentFrame(), COLLIDER_ROCK, "rock", (j1Module*)App->player);
	//set colliders to move the camera
	camera_toRight = App->collision->AddCollider({ position.x + 70,position.y - 100,20,140 }, COLLIDER_CAMERA, "right", (j1Module*)App->player);
	camera_toLeft = App->collision->AddCollider({ position.x - 50,position.y - 100,20,140 }, COLLIDER_CAMERA, "left", (j1Module*)App->player);
	camera_toUp = App->collision->AddCollider({ position.x - 50,position.y - 100,140,20 }, COLLIDER_CAMERA, "up", (j1Module*)App->player);
	camera_toDown = App->collision->AddCollider({ position.x - 50,position.y + 20,140,40 }, COLLIDER_CAMERA, "down", (j1Module*)App->player);

	App->audio->LoadFx("audio/fx/jump.wav");
	App->audio->LoadFx("audio/fx/hurt.wav");
	App->audio->LoadFx("audio/fx/throw.wav");


	//bar ability
	bar_0 = { 268,25,62,12 };
	bar_1 = { 268,41,62,12 };
	bar_2 = { 268,57,62,12 };
	bar_3 = { 268,73,62,12 };
	bar_4 = { 268,89,62,12 };

	initial_pos = position.x; //look to camera_toright. Position.x is the initial position at this moment

	bar_pos.x = 10;
	bar_pos.y = -130;

	return true;
}

bool j1Player::PreUpdate()
{
	LOG("coins: %i", collected_coins);
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
	player_input.pressing_F = App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
	player_input.pressing_lshift = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT;

	//states machine
	switch (state)
	{
	case entityStates::IDLE:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_space && in_air == false) //fix
		{
			restart_variables(-1, 1);
			state = entityStates::JUMP;
		}
		else if (player_input.pressing_D)
			state = entityStates::WALK_FORWARD;
		else if (player_input.pressing_A)
			state = entityStates::WALK_BACKWARD;

		break;
	case entityStates::WALK_FORWARD:
		restart_variables(-1, 1);

		if (!player_input.pressing_D)		
			if(moving_right) state = entityStates::DASH_FORWARD;
			else state = entityStates::IDLE;
			
		if (player_input.pressing_space && in_air == false)
			state = entityStates::JUMP_FORWARD;

		if (player_input.pressing_lshift)
			state = entityStates::RUN_FORWARD;

		looking_right = true;

		break;
	case entityStates::WALK_BACKWARD:
		restart_variables(-1, 1);

		if (!player_input.pressing_A)		
			if(moving_left)	state = entityStates::DASH_BACKWARD;
			else state = entityStates::IDLE;
		
		if (player_input.pressing_space && in_air == false)
			state = entityStates::JUMP_BACKWARD;

		if (player_input.pressing_lshift)
			state = entityStates::RUN_BACKWARD;

		looking_right = false;

		break;
	case entityStates::RUN_FORWARD:
		restart_variables(-1, 1);
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_D)
				state = entityStates::WALK_FORWARD;
			else
				state = entityStates::DASH_FORWARD;
		}
		else if (!player_input.pressing_D)
			state = entityStates::IDLE;

		if (player_input.pressing_space)
			state = entityStates::JUMP_FORWARD;

		looking_right = true;

		break;
	case entityStates::RUN_BACKWARD:
		restart_variables(-1, 1);
		if (!player_input.pressing_lshift)
		{
			if (player_input.pressing_A)
				state = entityStates::WALK_BACKWARD;
			else
				state = entityStates::DASH_BACKWARD;
		}
		else if (!player_input.pressing_A)
			state = entityStates::IDLE;

		if (player_input.pressing_space)
			state = entityStates::JUMP_BACKWARD;

		looking_right = false;

		break;
	case entityStates::JUMP:
		if (player_input.pressing_D)state = entityStates::JUMP_FORWARD;
		if (player_input.pressing_A)state = entityStates::JUMP_BACKWARD;

		break;
	case entityStates::JUMP_FORWARD:
		moving_right = true;
		moving_left = false;

		if (!player_input.pressing_D)state = entityStates::JUMP;
		break;
	case entityStates::JUMP_BACKWARD:
		moving_right = false;
		moving_left = true;
		if (!player_input.pressing_A)state = entityStates::JUMP;

		looking_right = false;

		break;
	case entityStates::DASH_FORWARD:
		moving_right = false;
		moving_left = false;
		if (player_input.pressing_A) {
			state = entityStates::WALK_BACKWARD;
			restart_variables(1, -1);
			if (player_input.pressing_lshift)state = entityStates::RUN_BACKWARD;
		}
		else if (!player_input.pressing_A)
		{
			restart_variables(1, -1);
			state = entityStates::IDLE;
		}

		break;
	case entityStates::DASH_BACKWARD:
		moving_right = false;
		moving_left = true;
		if (player_input.pressing_D) {
			state = entityStates::WALK_FORWARD;
			restart_variables(1, -1);
			if (player_input.pressing_lshift)state = entityStates::RUN_FORWARD;
		}
		else if (!player_input.pressing_D)
		{
			restart_variables(1, -1);
			state = entityStates::IDLE;
		}
		break;
	case entityStates::DIE:
		if (!livesUpdated)
		{
			lives--;
			livesUpdated = true;
		}

		state = entityStates::IDLE;
		break;
	}
	if (player_input.pressing_F) {
		throwRock();
	}

	//Change player collider position
	collider_entity->SetPos(position.x, position.y);
	//Change rock collider position
	collider_rock->SetPos(rockPosition.x, rockPosition.y);
	BROFILER_CATEGORY("Player_PreUpdate", Profiler::Color::Aquamarine);

	return true;
}

bool j1Player::Update(float dt)
{
	deltaTime = dt;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && in_air == false)	App->audio->PlayFx(1, 0); //sound of jumping before update

	checkAbility();//can we do the main ability?

	//State machine
	switch (state)
	{
	case entityStates::IDLE:
		current_animation = &idle;

		break;
	case entityStates::WALK_FORWARD:
		current_animation = &walk;

		if (collider_at_right == false)
		{
			position.x += (int)ceil(velocity * dt);
			moving_right = true;
		}
		else
			moving_right = false;

		break;
	case entityStates::WALK_BACKWARD:
		current_animation = &walk;

		if (collider_at_left == false || collider_at_right == true)
		{
			position.x -= (int)ceil(velocity * dt);
			moving_left = true;
		}
		else
			moving_left = false;

		break;
	case entityStates::RUN_FORWARD:
		current_animation = &run;
		if (collider_at_right == false)
		{
			moving_right = true;
			moving_left = false;
			position.x += (int)ceil(run_velocity * dt);
		}else if (collider_at_right == true)
		{
			moving_left = false;
			moving_right = false;
		}
		break;
	case entityStates::RUN_BACKWARD:
		current_animation = &run;
		if (collider_at_left == false)
		{
			moving_left = true;
			moving_right = false;
			position.x -= (int)ceil(run_velocity * dt);
		}else if (collider_at_left == true)
		{
			moving_left = false;
			moving_right = false;
		}
		break;
	case entityStates::JUMP:
		if (jump_vel > 0) {
			in_air = true;
			current_animation = &jump_up;
			jump_vel -= fall_velocity;
			position.y -= jump_vel;
		}
		else {
			current_animation = &jump_down;
			if (in_air == false) state = entityStates::IDLE;			
		}
		break;
	case entityStates::JUMP_FORWARD:
		if (jump_vel >= 0) {
			current_animation = &jump_up;
			in_air = true;
			jump_vel -= fall_velocity;
			position.y -= jump_vel;
		}
		else {
			current_animation = &jump_down;
			if (in_air == false) {
				state = entityStates::WALK_FORWARD;
				moving_right = true;
			}
		}
		if (collider_at_right == false)
			position.x += (int)ceil(velocity * dt);

		break;
	case entityStates::JUMP_BACKWARD:
		if (jump_vel >= 0) {
			current_animation = &jump_up;
			in_air = true;
			jump_vel -= fall_velocity;
			position.y -= jump_vel;
		}
		else {
			current_animation = &jump_down;
			if (in_air == false) {
				state = entityStates::WALK_BACKWARD;
				moving_left = true;
			}
		}
		if (collider_at_left == false)
			position.x -= (int)ceil(velocity * dt);
		break;
	case entityStates::DASH_FORWARD:
		current_animation = &walk;

		velocity -= (int)ceil(decrease_vel * dt);

		if (collider_at_right == false)
			position.x += (int)(velocity * dt);

		if (velocity <= 0) {
			moving_right = false;
			velocity = 2.0f;
			state = entityStates::IDLE;
		}
		break;
	case entityStates::DASH_BACKWARD:
		current_animation = &walk;

		velocity -= (int)ceil(decrease_vel * dt);

		if (collider_at_left == false)
			position.x -= (int)(velocity * dt);

		if (velocity <= 0)
		{
			moving_left = false;
			velocity = 2.0f;
			state = entityStates::IDLE;
		}
		break;
	case entityStates::DIE:
		ability_able = false;
		iterator = 0;
		App->fade->FadeToBlack(App->scene, App->scene);
		App->audio->PlayFx(2);
		break;
	}

	ApplyForces(dt);//Gravity and godmode
	rockMovement(dt);

	BlitEverything();

	if (player_input.pressing_A == true || player_input.pressing_D == true) //watching if the pj is walking
		walking = true;
	else walking = false;

	//if (walking == true)	App->audio->PlayFx(2, 1); //sound of walking active

	//do ability
	if (ability_able == true)
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			useAbility();
			tick4 = SDL_GetTicks();
		}

	BROFILER_CATEGORY("Player_Update", Profiler::Color::Beige);

	return true;
}

bool j1Player::CleanUp() {
	LOG("Unloading player\n");
	
	current_animation = nullptr;
	//Unload spritesheets
	{
		spritesheet_entity = nullptr;
		spritesheet_casper = nullptr;
		spritesheet_bars = nullptr;		
		spritesheet_rock = nullptr;
	}

	//Unload colliders
	{//Player
		collider_entity->to_delete = true;
		collider_entity = nullptr;
	}//Camera
	{
		camera_toRight->to_delete = true;
		camera_toRight = nullptr;
		camera_toLeft->to_delete = true;
		camera_toLeft = nullptr;
		camera_toUp->to_delete = true;
		camera_toUp = nullptr;
		camera_toDown->to_delete = true;
		camera_toDown = nullptr;
	}
	{//Rock
		collider_rock->to_delete = true;
		collider_rock = nullptr;
	}

	//Unload audio	
	App->audio->unLoadFx("audio/fx/jump.wav");

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	checkCameraColliders(c1, c2);

	if (godMode == false)
	{
		if (c1->type == COLLIDER_PLAYER)
		{
			switch (c2->type)
			{
			case COLLIDER_WALL:
				if (position.x + collider_entity->rect.w < c2->rect.x + 20) //Player is at the left of a wall				
					//There is a wall
					if (position.y + 0.7f * collider_entity->rect.h > c2->rect.y)collider_at_right = true;
					else collider_at_right = false;				
				else collider_at_right = false;

				if (position.x < c2->rect.x + c2->rect.w) //Player is at the right of a wall
				{
					//If player is looking backward
					if (state == entityStates::WALK_BACKWARD || state == entityStates::RUN_BACKWARD || state == entityStates::JUMP_BACKWARD || state == entityStates::IDLE || state == entityStates::DASH_BACKWARD)
						if (position.y + 0.7f * collider_entity->rect.h > c2->rect.y) collider_at_left = true;
						else collider_at_left = false;
				}
				else collider_at_left = false;

				break;
			case COLLIDER_DIE:
				state = entityStates::DIE;
				break;

			case COLLIDER_DOOR:
				//Change scene from 1 to 2
				if (c2->name == "door1")App->scene->choose_lv = 2;
				else if (c2->name == "door2")App->scene->choose_lv = 1;
				iterator = 0;
				App->fade->FadeToBlack(App->scene, App->scene);
				break;
			case COLLIDER_FLYING_ENEMY:
				state = entityStates::DIE;
				break;
			case COLLIDER_GROUND_ENEMY:
				state = entityStates::DIE;
				break;
			}
		}
	}
}

void j1Player::checkCameraColliders(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_CAMERA)
	{
		float localVelocity = 0;
		
		if (state == entityStates::RUN_FORWARD || state == entityStates::RUN_BACKWARD)localVelocity = run_velocity;
		else localVelocity = velocity;

		//Adjust velocity to delta time
		localVelocity = (int)ceil(localVelocity * deltaTime);

		if (c2->name == "right")MoveCameraColliders("x", localVelocity);		
		if (c2->name == "left")MoveCameraColliders("x", -localVelocity);		
		if (c2->name == "up")MoveCameraColliders("y", -localVelocity);		
		if (c2->name == "down")MoveCameraColliders("y", fall_velocity);		
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
			old_position[iterator].x = position.x;
			old_position[iterator].y = position.y;
			iterator++;
		}
		else {
			for (int i = 0; i <= 28; i++) { //if its full, we move the array
				old_position[i].x = old_position[i + 1].x;
				old_position[i].y = old_position[i + 1].y;
			}
			old_position[29].x = position.x; //what we pretend to do, is save the position of 1.5 sec before in time
			old_position[29].y = position.y;
		}
		tick2 = SDL_GetTicks();
	}
	tick1 = SDL_GetTicks();


	//blit bar
	if (tick3 - tick4 >= 1000) {
		if (tick3 - tick4 >= 2000) {
			if (tick3 - tick4 >= 3000) {
				App->render->Blit(spritesheet_bars, bar_pos.x, bar_pos.y, &bar_1);
			}
			else
				App->render->Blit(spritesheet_bars, bar_pos.x, bar_pos.y, &bar_2);
		}
		else
			App->render->Blit(spritesheet_bars, bar_pos.x, bar_pos.y, &bar_3);
	}
	else
		App->render->Blit(spritesheet_bars, bar_pos.x, bar_pos.y, &bar_4);

	if (ability_able == true)
		App->render->Blit(spritesheet_bars, bar_pos.x, bar_pos.y, &bar_0);

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
	//We move all the camera colliders
	camera_toRight->SetPos(position.x + 70, position.y - 100); //this +70 is added to initial pos too.
	camera_toLeft->SetPos(position.x - 50, position.y - 100);
	camera_toUp->SetPos(position.x - 50, position.y - 100);
	camera_toDown->SetPos(position.x - 50, position.y + 20);
	bar_pos.x = position.x - initial_pos + 82;//this 92 is the bar width + 20 initial

	position_when_ability = -position.x * screen_size + initial_pos;

	//remove magin numbers
	App->render->camera.x = position_when_ability; //we move the player position

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

	if (direction == "x") //Move right or left
	{
		if (speed < 0) //moving to left
			if (App->render->camera.x <= -1) { //Camera is inside the map
				App->render->camera.x -= 2 * speed;
				bar_pos.x += speed;
			}
		if (speed > 0) //moving to right
			if ((-App->render->camera.x + App->render->camera.w) <= App->map->data.width * 32) { //Camera is inside the map
				App->render->camera.x -= 2 * speed;
				bar_pos.x += speed;
			}

		camera_toRight->rect.x += speed;
		camera_toLeft->rect.x += speed;
		camera_toUp->rect.x += speed;
		camera_toDown->rect.x += speed;
	}
	else if (direction == "y") //Move up or down
	{
		if (speed < 0) { //moving up
			App->render->camera.y -= 2 * speed;
			bar_pos.y += speed;
		}
		if (speed > 0) //moving down
			if ((-App->render->camera.y + App->render->camera.h) < App->map->data.height * 32 - 16) { //Camera is inside the map
				App->render->camera.y -= 2 * speed;
				bar_pos.y += speed;
			}

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

		if (c2->type != COLLIDER_WALL) //We only want to check if player is colliding with a wall
			continue;
		if (collider_entity->CheckCollision(c2->rect) == false)
			continue;
		if (position.y > c2->rect.y)
			continue;
		if (position.x + 0.8 * collider_entity->rect.w < c2->rect.x)
			continue;
		if (position.x > c2->rect.x + c2->rect.w - 0.2 * collider_entity->rect.w)
			continue;

		//If we get here, player is grounded
		fall_velocity = 1;
		return false;
	}

	//We didn't found a collision with a wall, so in_air = true
	return true;
}

void j1Player::restart_variables(int vel, int vel_jump) {

	pugi::xml_node		node = App->GetConfig();

	if (vel != -1) {
		velocity = node.child("entityManager").child("player").child("velocity").attribute("value").as_float();
	}
	if (vel_jump != -1) {
		jump_vel = node.child("entityManager").child("player").child("jump_vel").attribute("value").as_float();
	}
}

void j1Player::rockMovement(float dt)
{
	if (rock_able == false) //If we have already thrown the rock
	{
		//We check if cooldown has finished or we continue with the rock
		if (rock_timer >= rock_cooldown) {
			//Delete rock
			rockPosition.x = -50;
			rockPosition.y = -50;
			rock_able = true; //We can throw the rock again
		}
		else
			rock_timer += dt; //We increase the countdown to make it available again

		//Rock movement
		if (rockCheckInAir() == true)
		{
			//We change the position based on the force 
			rockPosition.x += (int)ceil(rockVelocity.x * dt);
			rockPosition.y -= (int)ceil(rockVelocity.y * dt);

			//We apply gravity to the rock
			rock_fall_velocity += rock_gravity;
			rockPosition.y += (int)ceil(rock_fall_velocity * dt);
		}
		else
		{
			//Delete rock
			rockPosition.x = -5000;
			rockPosition.y = -5000;
		}
	}
}

void j1Player::throwRock()
{
	if (rock_able == true)
	{
		App->audio->PlayFx(3);
		rockPosition.y = position.y;
		rock_timer = 0;
		rock_able = false;

		//Rock movement
		//We calculate rock velocity depending on the state

		if (looking_right) {
			rockPosition.x = position.x + collider_entity->rect.w;
			if (rockVelocity.x < 0)
				rockVelocity.x *= -1;
		}
		else {
			rockPosition.x = position.x;
			rockVelocity.x *= -1;
			if (rockVelocity.x > 0)
				rockVelocity.x *= -1;
		}

	}
}

bool j1Player::rockCheckInAir()
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

		if (c2->type == COLLIDER_WALL) //We only want to check if player is colliding with a wall
			if (collider_rock->CheckCollision(c2->rect) == true) //There is collision between the rock and a wall			
				if (rockPosition.y < c2->rect.y) //Rock is on the ground
				{
					rock_fall_velocity = 0;
					return false;
				}
	}

	return true; //We didn't found a collision with a wall, so in_air = true. Player is not grounded
}

void j1Player::ApplyForces(float dt)
{
	//Gravity system
	if (godMode == false)
	{
		in_air = checkInAir();

		if (in_air == true)
		{
			if (fall_velocity < max_fall_velocity)
				fall_velocity += gravity * dt;

			position.y += (int)ceil(fall_velocity);
			//LOG("IN AIR");
		}
		else if (in_air == false)
		{
			//LOG("NOT IN AIR");
		}
	}
	//Activate godmode
	else if (godMode == true)
	{
		in_air = false;
		if (player_input.pressing_W)
			position.y -= (int)ceil(velocity * dt);
		if (player_input.pressing_S)
			position.y += (int)ceil(velocity * dt);
	}
}

void j1Player::BlitEverything()
{
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(spritesheet_casper, old_position[0].x, old_position[0].y, &idle.GetCurrentFrame());

	if (!looking_right)
		App->render->Blit(spritesheet_entity, position.x, position.y, &r, 1, 2); //looking at left
	else
		App->render->Blit(spritesheet_entity, position.x, position.y, &r); //looking at right			

	App->render->Blit(spritesheet_rock, rockPosition.x, rockPosition.y, &throw_rock.GetCurrentFrame());
}

void j1Player::CollectCoin()
{
	collected_coins++;
}
