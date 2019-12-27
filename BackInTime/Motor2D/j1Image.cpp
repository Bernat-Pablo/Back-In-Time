#include "j1Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UI_Elements.h"
#include "j1Player.h"

j1Image::j1Image() : UI_Elements()
{	
}

bool j1Image::Start()
{
	bool ret = true;

	texture = this->texture;
	position = this->pos;

	return ret;
}

bool j1Image::Update(float dt)
{
	bool ret = true;

	if (this->following_pj) {
		position.x = App->player->position.x;
		position.y = App->player->position.y;
	}
	else {
		position = this->pos;
	}

	Draw();

	return ret;
}

bool j1Image::CleanUp()
{
	bool ret = true;

	texture = nullptr;
	
	return ret;
}

void j1Image::Draw()
{
	App->render->Blit(texture, position.x + 20, position.y - 20);
}