#include <math.h>
#include "j1App.h"
#include "j1Fade.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "p2Log.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "SDL_mixer/include/SDL_mixer.h"

bool j1Fade::Awake(pugi::xml_node& c){
	int w = c.child("config").child("window").child("resolution").attribute("width").as_int();
	int h = c.child("config").child("window").child("resolution").attribute("height").as_int();
	screen = { 0, 0, w , h };

	return true;
}

j1Fade::~j1Fade()
{}

// Load assets
bool j1Fade::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	IsFading = false;
	return true;
}

// Update: draw background
bool j1Fade::Update(float dt)
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			//Fades music for 1 sec
			Mix_FadeOutMusic(1000);
			//Enables called module, disables current module


			moduleOff->Disable();
			App->player->Disable();
			moduleOn->Enable();


			//resets player & camera position
			App->render->camera.x = 0;
			App->render->camera.y = 0;

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		//	App->scene_john->resetstage = true;
		normalized = 1.0f - normalized;
		if (now >= total_time)
		{
			IsFading = false;
			current_step = fade_step::none;
		}
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1Fade::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	moduleOff = module_off;
	moduleOn = module_on;

	IsFading = true;

	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}