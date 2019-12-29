#include "j1Menu.h"
#include "j1Input.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include "j1RectSprites.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Player.h"
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
	heart = nullptr;

	heart_anim.PushBack({ 0, 0, 26, 26 },1.0f);
	heart_anim.PushBack({ 2, 5, 26, 26 },1.0f);
}

bool j1Menu::Awake(pugi::xml_node& conf)
{
	current_heart = &heart_anim;
	return true;
}

bool j1Menu::Start()
{
	App->render->camera = { 0,0 };
	heart = App->tex->Load("ui/heart.png");
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
		//if (ingame_UI_created == false)
		CreateInGameUI();
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

	//just testing
	App->gui->CreateUIElement(UI_Types::INPUTTEXT, 100, 200, "input", "0", false, "0", 100, 20);


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

	App->gui->DestroyUIElement("input");

	menu_created = false;
}

void j1Menu::CreateInGameMenu()
{
	App->gui->CreateUIElement(UI_Types::IMAGE, 0, 0, "background_in", "menu/menu_spritesheet.png", false);

	ingame_menu_created = true;
}

void j1Menu::DestroyInGameMenu()
{
	App->gui->DestroyUIElement("background_in");
	ingame_menu_created = false;
}

void j1Menu::CreateInGameUI()
{	
	{//Blit hearts
		switch(App->player->lives)
		{			
		case 3:
			App->render->Blit(heart, 50, 50, &current_heart->GetCurrentFrame());
			break;
		case 2:
			App->render->Blit(heart, 50, 50, &current_heart->GetCurrentFrame());
			break;
		case 1:
			App->render->Blit(heart, 50, 50, &current_heart->GetCurrentFrame());
			break;
		case 0:
			break;
		default:
			break;
		}	
	}
	//ingame_UI_created = true;
}

void j1Menu::DestroyInGameUI()
{

	//ingame_UI_created = false;
}