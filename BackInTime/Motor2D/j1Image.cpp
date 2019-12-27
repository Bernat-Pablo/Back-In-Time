#include "j1Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UI_Elements.h"
#include "j1Player.h"

j1Image::j1Image() : UI_Elements()
{	
	texture = App->gui->GetUISpritesheet();
}

bool j1Image::Start()
{
	bool ret = true;

	texture = App->gui->GetUISpritesheet();

	return ret;
}

bool j1Image::Update(float dt)
{
	bool ret = true;

	rect.x = App->player->position.x;
	rect.y = App->player->position.y;
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
	App->render->Blit(texture, rect.x, rect.y);
}