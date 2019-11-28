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
#include "j1PathFinding.h"

j1FlyingEnemy::j1FlyingEnemy() : j1Entity(entityTypes::FLYING_ENEMY)
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
	//velocity = 2.0f;
	//fall_velocity = 5.5f;
}

bool j1FlyingEnemy::Awake(pugi::xml_node& config)
{
	bool ret = true;

	config = App->GetConfig();
	config = config.child("entities").child("flyingEnemy");

	velocity = config.child("velocity").attribute("value").as_float();
	fall_velocity = config.child("fall_velocity").attribute("value").as_float();

	return ret;
}

bool j1FlyingEnemy::Start()
{
	//position.x = 752.0;
	position.x = 250.0;
	position.y = 30.0;
	spritesheet_entity = App->tex->Load("character/bird_spritesheet.png");
	debug_tex = App->tex->Load("maps/pathRect.png");
	state = entityStates::FLY;
	current_animation = &fly;
	collider_entity = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_FLYING_ENEMY, "bird", (j1Module*)App->flyingEnemy); //a collider to start

	return true;
}

bool j1FlyingEnemy::PreUpdate()
{
	//STATE MACHINE
	switch (state)
	{
	case entityStates::FLY:
		if (moving_right == true) {
			state = entityStates::FLY_FORWARD;
		}
		else if (moving_left == true) {
			state = entityStates::FLY_BACKWARD;
		}
		else if (falling == true) {
			state = entityStates::FALL;
		}
		break;
	case entityStates::FLY_FORWARD:
		if (moving_right == false) {
			state = entityStates::FLY;
		}
		break;
	case entityStates::FLY_BACKWARD:
		if (moving_left == false) {
			state = entityStates::FLY;
		}
		break;
	case entityStates::FALL:
		if (falling == false) {
			state = entityStates::IN_GROUND;
		}
		break;
	case entityStates::IN_GROUND:
		if (isgrounded == false) {
			state = entityStates::FLY_UP;
		}
		break;

	case entityStates::FLY_UP:


		break;
	}

	//PATH TO PLAYER (LOGIC)
	calculate_path();

	collider_entity->SetPos(position.x, position.y);

	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	isgrounded = false;
	//STATE MACHINE APPLYING MOVEMENT
	switch (state)
	{
	case entityStates::FLY:
		current_animation = &fly;

		break;
	case entityStates::FLY_FORWARD:
		current_animation = &fly;
		position.x += (int)ceil(velocity*dt);
		moving_right = true;
		moving_left = false;

		break;
	case entityStates::FLY_BACKWARD:
		current_animation = &fly;
		position.x -= (int)ceil(velocity * dt);
		moving_right = false;
		moving_left = true;

		break;
	case entityStates::FALL:
		current_animation = &fall;
		position.y += (int)ceil(fall_velocity * dt);
		falling = true;
		set_path = false;

		break;		

	case entityStates::IN_GROUND:
		if (!set_timer) {
			set_timer = true;
			tick2 = SDL_GetTicks();
		}
		current_animation = &ground;
		falling = false;
		isgrounded = true;
		set_path = false;
		tick1 = SDL_GetTicks();

		if (tick1 - tick2 >= 2500) {
			tick1 = tick2 = 0;
			isgrounded = false;
			position.y -= 7; //i have to put this to avoid collide to ground and set allways state to IN_GROUND
			set_timer = false;
		}

		break;

	case entityStates::FLY_UP:
		if (!set_timer) {
			set_timer = true;
			tick2 = SDL_GetTicks();
		}
		set_path = false;
		isgrounded = false;
		current_animation = &fly;
		tick1 = SDL_GetTicks();

		if (tick1 - tick2 >= 2500) {
			tick1 = tick2 = 0;
			state = entityStates::FLY;
			set_timer = false;
		}
		position.y -= (int)ceil(velocity * dt);
		break;

	default:
		break;
	}

	//BLIT
	App->render->Blit(spritesheet_entity, position.x, position.y, &current_animation->GetCurrentFrame());

	//PATH TO PLAYER (BLIT)
	blit_path();

	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(spritesheet_entity);
	App->tex->UnLoad(debug_tex);
	collider_entity = nullptr;
	return true;
}

void j1FlyingEnemy::calculate_path()
{
	iPoint origin = App->map->WorldToMap(App->player->position.x, App->player->position.y);

	iPoint p = App->render->ScreenToWorld(position.x, position.y);
	p = App->map->WorldToMap(position.x, position.y);
	if (position.x - App->player->position.x <=160 || position.x - App->player->position.x <= -160) {
		App->pathfinding->CreatePath(origin, p);
		if (set_path == true) {
			check_path_toMove();
		}
	}
}

void j1FlyingEnemy::blit_path()
{
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}
}

void j1FlyingEnemy::check_path_toMove()
{
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	iPoint pos = App->map->MapToWorld(path->At(1)->x, path->At(1)->y);

	if (pos.x < position.x) {
		state = entityStates::FLY_BACKWARD;
	}
	if (pos.x > position.x) {
		state = entityStates::FLY_FORWARD;
	}
	if (pos.x == position.x) {
		state = entityStates::FALL;
	}
}

void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2) {

	switch (c2->type)
	{
	case COLLIDER_WALL:
		if (position.y < c2->rect.y) {
			if (position.x + collider_entity->rect.w > c2->rect.x) {
				if (position.x < c2->rect.x + c2->rect.w - 0.2 * collider_entity->rect.w) {
					state = entityStates::IN_GROUND;
					isgrounded = true;
				}
			}
		}

		break;
	case COLLIDER_DIE:
		//TODO here we have to put -> delete enemy

		break;
	default:
		break;
	}

}
