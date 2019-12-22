#include "j1Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"

j1Coin::j1Coin() : j1Entity(entityTypes::COIN)
{
	type = entityTypes::COIN;

	float speed = 0.1f;
	//Rotate
	rotate.PushBack({ 0, 0, 26, 26 }, speed);
	rotate.PushBack({ 30, 0, 21, 26 }, speed);
	rotate.PushBack({ 63, 0, 14, 26 }, speed);
	rotate.PushBack({ 96, 0, 8, 26 }, speed);
	rotate.PushBack({ 124, 0, 14, 26 }, speed);
	rotate.PushBack({ 151, 0, 21, 26 }, speed);
}

j1Coin::~j1Coin(){}

bool j1Coin::Awake(pugi::xml_node& config)
{
	bool ret = true;

	current_animation = &rotate;

	return ret;
}

bool j1Coin::Start()
{
	bool ret = true;

	coin_texture = App->tex->Load("character/coin_spritesheet.png");
	collider_entity = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_COIN, "coin", (j1Module*)this);

	return ret;
}

bool j1Coin::PreUpdate()
{
	bool ret = true;

	collider_entity->SetPos(position.x, position.y);

	return ret;
}

bool j1Coin::Update(float dt)
{
	bool ret = true;

	App->render->Blit(coin_texture, position.x, position.y, &current_animation->GetCurrentFrame());
	
	return ret;
}

bool j1Coin::CleanUp()
{
	bool ret = true;
		
	coin_texture = nullptr;
	collider_entity->to_delete = true;
	collider_entity = nullptr;

	return ret;
}
