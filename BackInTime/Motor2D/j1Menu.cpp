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
	menuState = MAIN_MENU;
	menu_created = false;
	settings_created = false;
	ingame_menu_created = false;
	ingame_UI_created = false;
	background = nullptr;
}

bool j1Menu::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Menu::Start()
{
	App->render->camera = { 0,0 };
	CreateMenu();
	return true;
}

bool j1Menu::Update(float dt)
{
	switch(menuState)
	{
	case MAIN_MENU:
		if (menu_created == false)CreateMenu();
		
		break;
	case INGAME_MENU:
		if (ingame_menu_created == false)CreateInGameMenu();
		break;
	case INGAME_UI:
		if (ingame_UI_created == false)CreateInGameUI();
		break;
	case NONE:
		DestroyMenu();
		DestroyInGameMenu();
		DestroyInGameUI();
		break;
	default:
		break;
	}
	
	return true;
}

bool j1Menu::CleanUp()
{
	DestroyMenu();
	DestroyInGameMenu();
	DestroyInGameUI();
	
	return true;
}

void j1Menu::CreateMenu()
{
	App->gui->CreateUIElement(UI_Types::IMAGE, 0, 0, "background", "menu/menu_spritesheet.png", false);
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 60, "play", "0", false, "play");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 100, "continue", "0", false, "continue");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 140, "settings", "0", false, "setting");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 180, "credits", "0", false, "credits");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 220, "out", "0", false, "out");

	menu_created = true;
}

void j1Menu::DestroyMenu()
{
	App->gui->DestroyUIElement("background");
	App->gui->DestroyUIElement("play");
	App->gui->DestroyUIElement("continue");
	App->gui->DestroyUIElement("settings");
	App->gui->DestroyUIElement("credits");
	App->gui->DestroyUIElement("out");

	menu_created = false;
}

void j1Menu::CreateInGameMenu()
{

	ingame_menu_created = true;
}

void j1Menu::DestroyInGameMenu()
{

	ingame_menu_created = false;
}

void j1Menu::CreateInGameUI()
{

	ingame_UI_created = true;
}

void j1Menu::DestroyInGameUI()
{

	ingame_UI_created = false;
}