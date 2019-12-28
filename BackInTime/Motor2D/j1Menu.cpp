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
	App->gui->CreateUIElement(UI_Types::IMAGE, 0, 0, "menu/menu_spritesheet.png", false);
	App->gui->CreateUIElement(UI_Types::TEXT, 100, 0, "0", false, "hola");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, -100, "0", false, "hola");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, -60, "0", false, "hola");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, -20, "0", false, "hola");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 20, "0", false, "hola");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 60, "0", false, "hola");

	return true;
}

bool j1Menu::Update(float dt)
{
	if (menuAble) {		

		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
			ChangeMenuStatus();
			//ShellExecute(NULL, "open", "https://bernat-pablo.github.io/Back-In-Time/", NULL, NULL, SW_SHOWNORMAL);
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