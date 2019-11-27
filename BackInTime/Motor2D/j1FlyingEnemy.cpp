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

	float speed = 0.1f;
	
	//IDLE


	//GROUND


	//HIT


	//FALL

}

bool j1FlyingEnemy::Awake(pugi::xml_node&)
{
	return true;
}

bool j1FlyingEnemy::Start()
{
	return true;
}

bool j1FlyingEnemy::PreUpdate()
{
	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	return true;
}
