#include "Slider.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "Brofiler/Brofiler.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

bool Slider::Start()
{
	base.x = this->pos.x;
	base.y = this->pos.y;
	base.w = 3;
	base.h = 100;

	quad.w = 20;
	quad.h = 20;
	quad.y = base.y + 40;
	quad.x = base.x - 9;

	return true;
}

bool Slider::Update(float dt)
{
	App->render->DrawQuad(base, 255, 0, 0, 255);
	if(On())
		App->render->DrawQuad(quad, 0, 0, 155, 255);
	else App->render->DrawQuad(quad, 0, 0, 255, 255);

	if (OnClick()) {
		int nothing;
		int mouse_pos;
		App->input->GetMousePosition(nothing, mouse_pos);
		if(mouse_pos<base.y + base.h && mouse_pos>base.y)
			quad.y = mouse_pos-10;
		App->render->DrawQuad(quad, 0, 0, 55, 255);
	}

	slider = quad.y + 10 - base.y;
	BROFILER_CATEGORY("Slider_Update", Profiler::Color::Aquamarine);
	return true;
}

bool Slider::PostUpdate()
{
	//here it goes linking name to what u want to do
	if(name == "music")
	{
		Mix_VolumeMusic((int)slider*1.28);
	}else if(name == "fx")
	{
		p2List_item<Mix_Chunk*>* item = App->audio->fx.start;
		while (item != nullptr) {
			Mix_VolumeChunk(item->data, (int)slider * 1.28);
			item = item->next;
		}
	}
	BROFILER_CATEGORY("Slider_PostUpdate", Profiler::Color::Aquamarine);
	return true;
}

bool Slider::CleanUp()
{
	
	return true;
}

bool Slider::On()
{
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (mouse.x<quad.x + quad.w && mouse.x>quad.x) {
		if (mouse.y<quad.y + quad.h && mouse.y>quad.y) {
			return true;
		}
	}
	return false;
}

bool Slider::OnClick()
{
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (mouse.x<quad.x + quad.w && mouse.x>quad.x) {
		if (mouse.y<quad.y + quad.h && mouse.y>quad.y) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
				return true;
		}
	}
	return false;
}
