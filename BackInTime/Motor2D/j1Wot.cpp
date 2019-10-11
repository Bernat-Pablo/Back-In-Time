#include "j1Wot.h"

Wot::Wot() {
	texture = App->tex->Load("character/spritesheet_pj.png");

	idle.PushBack({ 32,0,17,27 });
	idle.PushBack({ 63,0,18,27 });
	idle.PushBack({ 94,0,20,26 });
	idle.loop = true;
	idle.speed = 0.1f;

}

bool Wot::Start() {

	return true;
}
bool Wot::CleanUp() {

	return true;
}
