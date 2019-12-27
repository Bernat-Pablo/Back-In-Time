#include "j1Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UI_Elements.h"

j1Image::j1Image() : UI_Elements()
{

}

bool j1Image::Update(float dt)
{
	bool ret = true;



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