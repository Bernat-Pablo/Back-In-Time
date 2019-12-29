#include "j1Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "Brofiler/Brofiler.h"

j1Coin::j1Coin() : j1Entity(entityTypes::COIN)
{
	type = entityTypes::COIN;

	coin_texture = nullptr;

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
	
	//Initialize variables from j1Coin.h
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

	BROFILER_CATEGORY("Coin_PreUpdate", Profiler::Color::Aquamarine);
	return ret;
}

bool j1Coin::Update(float dt)
{
	bool ret = true;

	App->render->Blit(coin_texture, position.x, position.y, &current_animation->GetCurrentFrame());
	
	BROFILER_CATEGORY("Coin_Update", Profiler::Color::Aquamarine);
	return ret;
}

bool j1Coin::PostUpdate()
{
	bool ret = true;

	BROFILER_CATEGORY("Coin_PostUpdate", Profiler::Color::Aquamarine);
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

void j1Coin::OnCollision(Collider* c1, Collider* c2) 
{
	switch (c2->type)
	{
	case COLLIDER_PLAYER:
		
		App->player->CollectCoin();
		App->entityManager->DestroyEntity(this);
		break;
	default:
		break;
	}
}