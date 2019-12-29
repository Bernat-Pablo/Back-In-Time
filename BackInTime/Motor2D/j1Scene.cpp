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
#include "j1PathFinding.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1Menu.h"

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
	int w, h;
	uchar* data = NULL;
	if(choose_lv==1)
		App->map->Load("maps/map_1.tmx");
	else if (choose_lv==2)
		App->map->Load("maps/map_2.tmx");
	//pathfinding
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
	debug_tex = App->tex->Load("maps/pathRect.png");

	App->audio->PlayMusic("audio/music/music.ogg");
	
	//UI_Element* text = App->gui->CreateUI_Element(UI_TypeElement::TEXT, 50, 50);
	//UI_Elements* image = App->gui->CreateUIElement(UI_Types::IMAGE, 100, 200);

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
	//Make the camera movement independent of framerate
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += (int)(100 * dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= (int)(100 * dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += (int)(100 * dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= (int)(100 * dt);
	
	//Start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{		
		startfromBeginning();
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

	if (App->input->GetKey(SDL_SCANCODE_O)==KEY_DOWN) {
		App->menu->menuAble = true;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
		
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->menu->ChangeMenuStatus("activate");

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->map->CleanUp();

	return true;
}

void j1Scene::startfromBeginning()
{
	pugi::xml_node config = App->GetConfig();

	if (choose_lv == 1)
	{
		App->player->position.x = config.child("player").child("initialPosition").child("map1").attribute("x").as_int();
		App->player->position.y = config.child("player").child("initialPosition").child("map1").attribute("y").as_int();

		App->render->camera.x = config.child("render").child("camera_initialPosition").child("map1").attribute("x").as_int();
		App->render->camera.y = config.child("render").child("camera_initialPosition").child("map1").attribute("y").as_int();
	}
	else if (choose_lv == 2)
	{
		App->player->position.x = config.child("player").child("initialPosition").child("map2").attribute("x").as_int();
		App->player->position.y = config.child("player").child("initialPosition").child("map2").attribute("y").as_int();

		App->render->camera.x = config.child("render").child("camera_initialPosition").child("map2").attribute("x").as_int();
		App->render->camera.y = config.child("render").child("camera_initialPosition").child("map2").attribute("y").as_int();
	}
}
