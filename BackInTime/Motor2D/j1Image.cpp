#include "j1Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UI_Elements.h"
#include "j1Player.h"
#include "Brofiler/Brofiler.h"

j1Image::j1Image() : UI_Elements()
{	
	texture = nullptr;
}

bool j1Image::Start()
{
	bool ret = true;
	
	texture = App->tex->Load(this->texture_path);
	position = local_position = this->pos;

	return ret;
}

bool j1Image::Update(float dt)
{
	bool ret = true;

	if (this->following_pj) {
		//position.x = App->player->position.x;
		//position.y = App->player->position.y;
		
		position.x = -App->render->camera.x + local_position.x;
		position.y = App->render->camera.y + local_position.y;
		LOG("camera x: %i", App->render->camera.x);
		LOG("camera y: %i", App->render->camera.y);
		LOG("local x: %i", local_position.x);
		LOG("local y: %i", local_position.y);
	}
	else {
		position = this->pos;
	}

	Draw();

	BROFILER_CATEGORY("Image_Update", Profiler::Color::Aquamarine);
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
	App->render->Blit(texture, position.x, position.y);
}