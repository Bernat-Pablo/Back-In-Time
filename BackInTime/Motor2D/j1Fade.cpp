#include <math.h>
#include "j1App.h"
#include "j1Fade.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Brofiler/Brofiler.h"
#include "j1FlyingEnemy.h"

bool j1Fade::Awake(pugi::xml_node& c){
	doc.load_file("config.xml");
	int w = doc.child("config").child("window").child("resolution").attribute("width").as_int();
	int h = doc.child("config").child("window").child("resolution").attribute("height").as_int();
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
			
			//ugly but working
			
			//We call to Cleanups
			App->collision->CleanUp(); //Delete previous colliders
			App->player->CleanUp();
			App->scene->CleanUp(); //Clean up map			
			App->audio->CleanUp();
			App->tex->CleanUp();
			App->flyingEnemy->CleanUp();
			

			//We load new map	
			App->tex->Awake(App->GetConfig());
			App->audio->Awake(App->GetConfig());
			App->scene->Awake();
			App->scene->Start();
			App->player->Start(); //Set initial player position
			App->collision->Awake(App->GetConfig());	
			App->render->cameraSetInitialPosition(App->GetConfig()); //Set initial camera position
			App->flyingEnemy->Start();
			

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

	BROFILER_CATEGORY("Fade_Update", Profiler::Color::BlanchedAlmond);

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