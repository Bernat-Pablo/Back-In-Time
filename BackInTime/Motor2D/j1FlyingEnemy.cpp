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

j1FlyingEnemy::j1FlyingEnemy() : j1Module()
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

}

bool j1FlyingEnemy::Awake(pugi::xml_node&)
{
	return true;
}

bool j1FlyingEnemy::Start()
{
	spritesheet = App->tex->Load("character/bird_spritesheet.png");
	current_animation = &fall;
	return true;
}

bool j1FlyingEnemy::PreUpdate()
{
	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	App->render->Blit(spritesheet, 50, 50, &current_animation->GetCurrentFrame());
	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	return true;
}
