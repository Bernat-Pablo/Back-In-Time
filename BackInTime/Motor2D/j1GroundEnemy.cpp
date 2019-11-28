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
