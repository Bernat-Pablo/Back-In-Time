#include "j1Menu.h"
#include "j1Input.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include "j1RectSprites.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Player.h"
#include <windows.h>
#include "Brofiler/Brofiler.h"
#include "j1Fonts.h"
#include "j1FontUI.h"

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
	want_ingame_ui = false;

	heart_anim.PushBack({ 0, 0, 26, 26 },1.0f);
	heart_anim.PushBack({ 2, 5, 26, 26 },1.0f);
	heart_anim.loop = true;
}

bool j1Menu::Awake(pugi::xml_node& conf)
{
	
	return true;
}

bool j1Menu::Start()
{
	current_heart = &heart_anim;
	App->render->camera = { 0,0 };
	heart = App->tex->Load("character/spritesheet_pj.png");
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
		want_ingame_ui = true;
		if (ingame_UI_created == false)CreateInGameUI();
		break;
	case NONE:
		DestroyAllUI();
		break;
	default:
		break;
	}	

	BROFILER_CATEGORY("Menu_Update", Profiler::Color::Aquamarine);
	return true;
}

bool j1Menu::CleanUp()
{
	DestroyAllUI();
	
	return true;
}

void j1Menu::CreateMenu()
{
	App->gui->CreateUIElement(UI_Types::IMAGE, 0, 0, "background", "menu/cover_resized.png", false);
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 60, "play", "0", false, "play");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 100, "continue", "0", false, "continue");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 140, "settings", "0", false, "setting");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 180, "credits", "0", false, "credits");
	App->gui->CreateUIElement(UI_Types::BUTTON, 380, 220, "out", "0", false, "out");

	//just testing


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
	int pos_x = App->player->position.x; 
	int pos_y = App->player->position.y;
	App->gui->CreateUIElement(UI_Types::IMAGE, pos_x + 20, pos_y -300, "background_in", "menu/ingame_menu_spritesheet.png", false);
	App->gui->CreateUIElement(UI_Types::BUTTON, pos_x + 100, pos_y -200, "resume", "0", false, "resume");
	App->gui->CreateUIElement(UI_Types::BUTTON, pos_x + 100, pos_y -150, "settings", "0", false, "setting");
	App->gui->CreateUIElement(UI_Types::BUTTON, pos_x + 100, pos_y -100, "main menu", "0", false, "main menu");

	//App->gui->CreateUIElement(UI_Types::SLIDER, pos_x + 20, pos_y -180, "music");
	//App->gui->CreateUIElement(UI_Types::SLIDER, pos_x + 120, pos_y -150, "fx");
	//App->gui->CreateUIElement(UI_Types::TEXT, pos_x + 0, pos_y + 90, "music_text", "0", false, "music");
	//App->gui->CreateUIElement(UI_Types::TEXT, pos_x + 92, pos_y + 90, "fx_text", "0", false, "effects");

	ingame_menu_created = true;
}

void j1Menu::DestroyInGameMenu()
{
	App->gui->DestroyUIElement("background_in");
	App->gui->DestroyUIElement("resume");
	App->gui->DestroyUIElement("settings");
	App->gui->DestroyUIElement("main menu");
	//App->gui->DestroyUIElement("music");
	//App->gui->DestroyUIElement("fx");
	//App->gui->DestroyUIElement("music_text");
	//App->gui->DestroyUIElement("fx_text");
	ingame_menu_created = false;
}

void j1Menu::CreateInGameUI()
{	
	//Lives
	iPoint heartPos[3] = {
		{10, 0}, //heart 1
		{50, 0}, //heart 2
		{90, 0} //heart 3
	};

	for (size_t i = 0; i < App->player->lives; i++)
	{
		j1Image* heart = (j1Image*)App->gui->CreateUIElement(UI_Types::IMAGE, heartPos[i].x, heartPos[i].y, "heart", "ui/heart_resized.png", true);
		screen_ui.add(heart);
	}

	//Coins
	iPoint coinPos = { 200, 20 };

	for (size_t i = 0; i < App->player->collected_coins; i++)
	{
		j1Image* coin = (j1Image*)App->gui->CreateUIElement(UI_Types::IMAGE, coinPos.x + (20 * i), coinPos.y, "coin", "ui/coin.png", true);
		screen_ui.add(coin);
	}

	//Timer
	iPoint timerPos = { 200, 20 };
	Font_UI* timer = (Font_UI*)App->gui->CreateUIElement(UI_Types::TEXT, timerPos.x, timerPos.y, "timer", "0", true, "eins1");
	
	App->gui->CreateUIElement(UI_Types::TEXT, App->player->position.x, App->player->position.y, "music_text", "0", false, "zwei2");
	App->gui->CreateUIElement(UI_Types::TEXT, 80, 220, "music_text", "0", false, "drei3");
	App->gui->CreateUIElement(UI_Types::TEXT, 172, 220, "fx_text", "0", false, "vier4");

	ingame_UI_created = true;
}

void j1Menu::DestroyInGameUI()
{

	ingame_UI_created = false;
}

void j1Menu::UpdateInGameUI()
{
	p2List_item<j1Image*>* image = nullptr;

	//App->gui->DestroyUIElement("coin");
	//App->gui->DestroyUIElement("heart");

	CreateInGameUI();
}

void j1Menu::DestroyAllUI()
{
	DestroyInGameMenu();
	DestroyInGameUI();
	DestroyMenu();
}
