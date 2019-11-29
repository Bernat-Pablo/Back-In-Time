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

bool j1GroundEnemy::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GroundEnemy::Start()
{

	return true;
}

bool j1GroundEnemy::PreUpdate()
{
	return true;
}

bool j1GroundEnemy::Update(float dt)
{
	return true;
}

bool j1GroundEnemy::CleanUp()
{
	return true;
}

void j1GroundEnemy::OnCollision(Collider* c1, Collider* c2)
{

}

void j1GroundEnemy::calculate_path()
{

}

void j1GroundEnemy::blit_path()
{

}

void j1GroundEnemy::check_path_toMove()
{

}
