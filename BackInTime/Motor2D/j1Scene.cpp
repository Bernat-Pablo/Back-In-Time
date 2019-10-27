#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Fade.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(choose_lv==1)
		App->map->Load("maps/map_1.tmx");
	else if (choose_lv==2)
		App->map->Load("maps/map_2.tmx");

	App->audio->PlayMusic("audio/music/music.ogg");
	//App->map->Load("iso.tmx");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//Change to Lv1
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		choose_lv = 1;
		App->fade->FadeToBlack(App->scene, App->scene);
	}

	//Change to Lv2
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		choose_lv = 2;
		App->fade->FadeToBlack(App->scene, App->scene);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{	
	pugi::xml_node config = App->GetConfig();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;
	
	//Start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		if(choose_lv == 1)
		{
			App->player->position.x = config.child("player").child("initialPosition").child("map1").attribute("x").as_int();
			App->player->position.y = config.child("player").child("initialPosition").child("map1").attribute("y").as_int();
			
			App->render->camera.x = config.child("render").child("camera_initialPosition").child("map1").attribute("x").as_int();
			App->render->camera.y = config.child("render").child("camera_initialPosition").child("map1").attribute("y").as_int();
		}else if(choose_lv == 2)
		{
			App->player->position.x = config.child("player").child("initialPosition").child("map2").attribute("x").as_int();
			App->player->position.y = config.child("player").child("initialPosition").child("map2").attribute("y").as_int();

			App->render->camera.x = config.child("render").child("camera_initialPosition").child("map2").attribute("x").as_int();
			App->render->camera.y = config.child("render").child("camera_initialPosition").child("map2").attribute("y").as_int();
		}
		
	}

	//Save the current state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	//Load the previous state (even across levels)
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");
	
	//Activate God mode
	//if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)

	App->map->Draw();


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->map->CleanUp();

	return true;
}
