#include "j1Menu.h"
#include "j1Input.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"
#include "j1RectSprites.h"
#include "j1App.h"
#include "j1Gui.h"
#include <windows.h>

j1Menu::j1Menu()
{
	name.create("menu");
}

bool j1Menu::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Menu::Start()
{
	App->render->camera = { 0,0 };
	App->gui->CreateUIElement(UI_Types::IMAGE, 0, 0, "background","menu/menu_spritesheet.png", false);

	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 60, "play", "0", false, "play");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 100,"continue", "0", false, "continue");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 140, "settings","0", false, "setting");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 180, "credits","0", false, "credits");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 220, "out","0", false, "out");

	App->gui->DestroyUIElement("continue");
	return true;
}

bool j1Menu::Update(float dt)
{
	if (menuAble) {		

		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
		}
	}
	return true;
}

bool j1Menu::CleanUp()
{
	return true;
}

void j1Menu::ChangeMenuStatus()
{
	menuAble = !menuAble;
}