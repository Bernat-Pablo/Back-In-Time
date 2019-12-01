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
	type = entityTypes::FLYING_ENEMY;

	float speed = 0.55f;
	
	//FLY
	fly.PushBack({ 24,0,35,39 },speed);
	fly.PushBack({ 63,0,35,39 }, speed);
	fly.PushBack({ 103,0,35,45 }, speed);
	fly.PushBack({ 143,0,35,45 }, speed);
	fly.PushBack({ 183,0,35,39 }, speed);
	fly.PushBack({ 223,0,35,39 }, speed);
	fly.PushBack({ 263,0,35,39 }, speed);
	fly.PushBack({ 303,0,35,39 }, speed);

	//GROUND
	speed = 0.2f;
	ground.PushBack({ 24,45,37,36 }, speed);
	ground.PushBack({ 64,45,37,36 }, speed);
	ground.PushBack({ 104,45,37,36 }, speed);
	ground.PushBack({ 144,45,37,36 }, speed);

	//HIT
	speed = 0.3f; 
	hit.PushBack({ 23,126,39,39 }, speed);
	hit.PushBack({ 64,126,39,39 }, speed);
	hit.PushBack({ 104,126,39,39 }, speed);
	hit.PushBack({ 143,126,39,39 }, speed);
	hit.PushBack({ 184,126,39,39 }, speed);
	hit.loop = false;
	
	//FALL
	speed = 0.3f;
	fall.PushBack({ 24,81,35,45 }, speed);
	fall.PushBack({ 64,81,35,45 }, speed);
	fall.PushBack({ 104,81,35,45 }, speed);
	fall.PushBack({ 144,81,35,45 }, speed);	
}

bool j1FlyingEnemy::Awake(pugi::xml_node& config)
{
	bool ret = true;

	config = App->GetConfig();
	config = config.child("entityManager").child("flyingEnemy");

	velocity = config.child("velocity").attribute("value").as_float();
	fall_velocity = config.child("fall_velocity").attribute("value").as_float();

	return ret;
}

bool j1FlyingEnemy::Start()
{
	spritesheet_entity = App->tex->Load("character/enemies_spritesheet.png");
	debug_tex = App->tex->Load("maps/pathRect.png");
	state = entityStates::FLY;

	current_animation = &fly;
	collider_entity = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_FLYING_ENEMY, "bird", (j1Module*)this); //a collider to start

	isgrounded = false;

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
		if (starting_flying == true) {
			state = entityStates::FLY_UP;
		}
		break;

	case entityStates::HIT:
		if (!being_hit)
			state = entityStates::FLY;

		break;
	}

	//PATH TO PLAYER (LOGIC)
	calculate_path();

	BROFILER_CATEGORY("Bird_PreUpdate", Profiler::Color::DarkGray);

	return true;
}

bool j1FlyingEnemy::Update(float dt)
{	
	//STATE MACHINE APPLYING MOVEMENT
	switch (state)
	{
	case entityStates::FLY:
		current_animation = &fly;
		starting_flying = false;
		set_path = true;

		break;
	case entityStates::FLY_FORWARD:
		current_animation = &fly;
		starting_flying = false;
		position.x += (int)ceil(velocity*dt);
		moving_right = true;
		moving_left = false;

		break;
	case entityStates::FLY_BACKWARD:
		current_animation = &fly;
		position.x -= (int)ceil(velocity * dt);
		moving_right = false;
		starting_flying = false;
		moving_left = true;

		break;
	case entityStates::FALL:
		current_animation = &fall;
		position.y += (int)ceil(fall_velocity * dt);
		falling = true;
		set_path = false;
		moving_right = false;
		moving_left = false;

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
			starting_flying = true;
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

		if (tick1 - tick2 >= 2000) {
			tick1 = tick2 = 0;
			state = entityStates::FLY;
			set_timer = false;
		}
		position.y -= (int)ceil(velocity * dt);
		break;

	case entityStates::HIT:
		current_animation = &hit;
		hit.Reset();

		break;
	default:
		break;
	}

	//BLIT
	App->render->Blit(spritesheet_entity, position.x, position.y, &current_animation->GetCurrentFrame());
	if(collider_entity != nullptr)
		collider_entity->SetPos(position.x, position.y);

	
	//PATH TO PLAYER (BLIT)
	if(App->collision->debug)
		blit_path();

	BROFILER_CATEGORY("Bird_Update", Profiler::Color::Fuchsia);

	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(spritesheet_entity);
	App->tex->UnLoad(debug_tex);
	collider_entity = nullptr;
	spritesheet_entity = nullptr;
	return true;
}

void j1FlyingEnemy::calculate_path()
{
	iPoint origin = App->map->WorldToMap(App->player->position.x, App->player->position.y);

	iPoint p = App->render->ScreenToWorld(position.x, position.y);
	p = App->map->WorldToMap(position.x, position.y);
	if (App->player->position.x-position.x<=160 && position.x - App->player->position.x >=-160) {
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
	if (pos.x >= position.x-5 && pos.x <= position.x + 5) {
		state = entityStates::FALL;
	}
}

void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2) {

	if(c1->type == COLLIDER_FLYING_ENEMY)
	{
		switch (c2->type)
		{
		case COLLIDER_WALL:
			if (position.y < c2->rect.y) {
				if(collider_entity != nullptr)
					if (position.x + collider_entity->rect.w > c2->rect.x) {
						if (position.x < c2->rect.x + c2->rect.w - 0.2 * collider_entity->rect.w) {
							state = entityStates::IN_GROUND;
							if (!starting_flying)
								isgrounded = true;
						}
					}
			}
			break;
		case COLLIDER_ROCK:
			App->entityManager->DestroyEntity(this);
			break;
		case COLLIDER_DIE:
			App->entityManager->DestroyEntity(this);
			break;
		default:
			break;
		}
	}	
}
