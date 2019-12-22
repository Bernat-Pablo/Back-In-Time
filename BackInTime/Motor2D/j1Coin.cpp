#include "j1Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

j1Coin::j1Coin() : j1Entity(entityTypes::COIN)
{
	type = entityTypes::COIN;

	float speed = 0.1f;
	//Rotate
	rotate.PushBack({ 20,39,170,171 }, speed);
	rotate.PushBack({ 229,39,131,171 }, speed);
	rotate.PushBack({ 449,39,81,171 }, speed);
	rotate.PushBack({ 669,39,41,171 }, speed);
	rotate.PushBack({ 859,39,81,171 }, speed);
	rotate.PushBack({ 1039,39,131,171 }, speed);
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

	return ret;
}

bool j1Coin::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Coin::Update(float dt)
{
	bool ret = true;

	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(coin_texture, position.x, position.y);

	return ret;
}

bool j1Coin::CleanUp()
{
	bool ret = true;

	coin_texture = nullptr;

	return ret;
}
