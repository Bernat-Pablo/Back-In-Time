#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1RectSprites.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Animation.h"
#include "p2Log.h"

j1Player::j1Player() : j1Module(){

	//IDLE
	idle.PushBack({ 0,0,17,26 });
	idle.PushBack({ 32,0,17,27 });
	idle.PushBack({ 63,0,18,28 });
	idle.PushBack({ 94,0,19,26 });

	idle.speed = 0.8f;

	//WALK
	walk.PushBack({ 0,27,17,25 });
	walk.PushBack({ 32,27,17,26 });
	walk.PushBack({ 64,27,17,27 });
	walk.PushBack({ 95,29,18,26 });
	walk.PushBack({ 128,28,17,26 });
	walk.PushBack({ 160,27,17,27 });
	
	walk.speed = 0.8f;

	//JUMP
	jump.PushBack({ 0,58,17,26 });
	jump.PushBack({ 30,60,20,24 });
	jump.PushBack({ 61,60,22,24 });
	jump.PushBack({ 96,55,17,29 });
	jump.PushBack({ 127,54,18,27 });
	jump.PushBack({ 158,56,20,27 });
	jump.PushBack({ 189,60,22,24 });
	jump.PushBack({ 223,58,18,26 });

	jump.speed = 0.8f;

}

bool j1Player::Awake(pugi::xml_node& config){

	spritesheet_pj = App->tex->Load("character/spritesheet_pj.png");
	

	return true;
}

bool j1Player::Update(float dt) {

	current_animation = &idle;

	RectSprites r = current_animation->GetCurrentFrame();

	App->render->Blit(spritesheet_pj, x, y, &r.rect);

	return true;
}



