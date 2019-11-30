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
#include "j1GroundEnemy.h"

j1GroundEnemy::j1GroundEnemy() : j1Entity(entityTypes::GROUND_ENEMY)
{
	name.create("goundEnemy");

	speed = 0.5f;

	idle.PushBack({ 23,165,50,29 }, speed);
	idle.PushBack({ 75,165,50,29 }, speed);
	idle.PushBack({ 127,165,50,29 }, speed);
	idle.PushBack({ 179,165,50,29 }, speed);
	idle.PushBack({ 232,165,50,29 }, speed);
	idle.PushBack({ 284,165,50,29 }, speed);
	idle.PushBack({ 335,165,50,29 }, speed);
	idle.PushBack({ 388,165,50,29 }, speed);
	idle.PushBack({ 440,165,50,29 }, speed);
	idle.PushBack({ 492,165,50,29 }, speed);
	idle.PushBack({ 544,165,50,29 }, speed);

	run.PushBack({ 24,198,50,30 }, speed);
	run.PushBack({ 76,198,50,30 }, speed);
	run.PushBack({ 128,198,50,30 }, speed);
	run.PushBack({ 180,198,50,30 }, speed);
	run.PushBack({ 223,198,50,30 }, speed);
	run.PushBack({ 284,198,50,30 }, speed);

	hit.PushBack({ 22,234,44,36 }, speed);
	hit.PushBack({ 74,234,44,36 }, speed);
	hit.PushBack({ 125,234,44,36 }, speed);
	hit.PushBack({ 176,234,44,36 }, speed);

}

bool j1GroundEnemy::Awake(pugi::xml_node& config)
{
	config = App->GetConfig();
	config = config.child("entityManager").child("groundEnemy");

	//variables definition
	
	return true;
}

bool j1GroundEnemy::Start()
{

	spritesheet_entity = App->tex->Load("character/enemies_spritesheet.png");
	debug_tex = App->tex->Load("maps/pathRect.png");

	state = entityStates::IDLE;
	current_animation = &fly;

	collider_entity = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_GROUND_ENEMY, "rino", (j1Module*)App->groundEnemy); //a collider to start

	return true;
}

bool j1GroundEnemy::PreUpdate()
{
	switch (state)
	{
	case entityStates::IDLE:
		if (moving_left)
			state = entityStates::RUN_BACKWARD;
		if (moving_right)
			state = entityStates::RUN_FORWARD;

		break;
	case entityStates::RUN_FORWARD:
		if(stun)
			state = entityStates::STUNNED;

		break;
	case entityStates::RUN_BACKWARD:
		if (stun)
			state = entityStates::STUNNED;

		break;
	case entityStates::STUNNED:
		if (!stun)
			state = entityStates::IDLE;

		break;
	case entityStates::HIT:
		if(!being_hit)
			state = entityStates::IDLE;

		break;
	}

	return true;
}

bool j1GroundEnemy::Update(float dt)
{
	switch (state)
	{
	case entityStates::IDLE:

		break;
	case entityStates::RUN_FORWARD:

		break;
	case entityStates::RUN_BACKWARD:

		break;
	case entityStates::STUNNED:

		break;
	case entityStates::HIT:

		break;
	}

	//BLIT
	App->render->Blit(spritesheet_entity, position.x, position.y, &current_animation->GetCurrentFrame());

	return true;
}

bool j1GroundEnemy::CleanUp()
{
	App->tex->UnLoad(spritesheet_entity);
	App->tex->UnLoad(debug_tex);

	return true;
}

void j1GroundEnemy::OnCollision(Collider* c1, Collider* c2)
{

}

void j1GroundEnemy::calculate_path()
{
	iPoint origin = App->map->WorldToMap(App->player->position.x, App->player->position.y);

	iPoint p = App->render->ScreenToWorld(position.x, position.y);
	p = App->map->WorldToMap(position.x, position.y);
	if (App->player->position.x - position.x <= 160 && position.x - App->player->position.x >= -160) {
		App->pathfinding->CreatePath(origin, p);
		if (set_path == true) {
			check_path_toMove();
		}
	}
}

void j1GroundEnemy::blit_path()
{
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}
}

void j1GroundEnemy::check_path_toMove()
{

}
