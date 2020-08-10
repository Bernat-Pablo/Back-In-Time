#include "j1Console.h"
#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1Input.h"
#include "p2SString.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Fade.h"


j1Console::j1Console()
{
	name.create("console");
	god = "god_mode";
	quit = "quit";
	fps = "fps";
	map = "map";
	list = "god_mode,quit,fps<>,map<>";
}

bool j1Console::Awake()
{
	return true;
}

bool j1Console::Start()
{
	return true;
}

bool j1Console::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Console_PreUpdate", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) {
		open = !open;
	}
	if (open) {
		App->render->DrawQuad({ 200,0,200,200 }, 0, 0, 255, 255);
		App->gui->CreateUIElement(UI_Types::INPUTTEXT, 200, 180, "console", "0", false, "0",false, 200);
		App->is_paused = true;
	}
	else {
		App->gui->DestroyUIElement("console");
		App->is_paused = false;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RETURN)){
		text = App->input->text_frominput;
		const char* string = text.GetString();
		if (text == "godmode on") {
			App->player->godMode = true;
			App->player->collider_at_left = false;
			App->player->collider_at_right = false;
		}
		else if (text == "godmode off") {
			App->player->godMode = false;
		}
		else if (text == "fps60") {
			App->cap_framerate = false;
		}
		else if (text == "fps30") {
			App->cap_framerate = true;
		}
		else if (text == "map1") {
			App->scene->choose_lv = 1;
			App->fade->FadeToBlack(App->map, App->map);
		}
		else if (text == "map2") {
			App->scene->choose_lv = 2;
			App->fade->FadeToBlack(App->map, App->map);
		}
		else if (text == "quit") {
			return false;
		}
		App->input->text_frominput.Clear();
	}
	BROFILER_CATEGORY("Console_Update", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Console_PostUpdate", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

