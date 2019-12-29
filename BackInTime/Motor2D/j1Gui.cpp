#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Image.h"
#include "j1Textures.h"
#include "j1FontUI.h"
#include "j1Button.h"
#include "j1Render.h"

j1Gui::j1Gui()
{
	name.create("gui");
}

bool j1Gui::Awake(pugi::xml_node& config)
{
	bool ret = true;

	ui_spritesheet_path = config.child("texture").attribute("path").as_string("");
	//ui_spritesheet = App->tex->Load("character/spritesheet_pj.png");

	return ret;
}

bool j1Gui::Start()
{
	bool ret = true;

	ret = true;
	ui_spritesheet = App->tex->Load(ui_spritesheet_path.GetString());
	App->fonts->Load("fonts/small_white_font.png", " abcdefghiklmnoprstuwy'.0123456789", 1, 8,8,34);
	//ui_spritesheet = App->tex->Load("character/coin_spritesheet.png");

	return ret;
}

bool j1Gui::PreUpdate()
{
	bool ret = true;

	p2List_item<UI_Elements*>* element = ui_elementsList.start;
	while (element != nullptr)
	{		
		if (element->data->PreUpdate() == false) ret = false;
		element = element->next;
	}

	return ret;
}

bool j1Gui::Update(float dt)
{
	bool ret = true;

	p2List_item<UI_Elements*>* element = ui_elementsList.start;
	while (element != nullptr)
	{
		if (element->data->Update(dt) == false) ret = false;
		element = element->next;
	}

	return ret;
}

bool j1Gui::PostUpdate()
{
	bool ret = true;

	p2List_item<UI_Elements*>* element = ui_elementsList.start;
	while (element != nullptr)
	{
		if (element->data->PostUpdate() == false) ret = false;
		element = element->next;
	}
	if (changing) {
		App->render->DrawQuad({ -200,-200,1000,1000 }, 0, 0, 0, 255);
	}

	return ret;
}

bool j1Gui::CleanUp()
{
	bool ret = true;

	ui_spritesheet = nullptr;

	return ret;
}

UI_Elements* j1Gui::CreateUIElement(UI_Types type, int position_x, int position_y,char* name, char* texture, bool follow_pj, char* t)
{
	UI_Elements* ret = nullptr;

	switch (type)
	{
	case UI_Types::TEXT:
		ret = new Font_UI();
		if (follow_pj)
			ret->following_pj = follow_pj;
		ret->text_font = t;
		ret->pos.x = position_x;
		ret->pos.y = position_y;
		ret->name = name;
		ret->Start();
		break;
	case UI_Types::IMAGE:
		ret = new j1Image();
		if (texture != "0")
			ret->texture_path = texture;
		if (follow_pj)
			ret->following_pj = follow_pj;
		ret->pos.x = position_x;
		ret->pos.y = position_y;
		ret->name = name;
		ret->Start();
		break;
	case UI_Types::BUTTON:
		ret = new Button();
		ret->margeButton.y = 11;
		ret->numberLetters = strlen(t);
		ret->margeButton.x = -ret->numberLetters*4+40;
		ret->rect = { position_x ,position_y, 80,30 };
		ret->text_font = t;
		ret->name = name;
		ret->Start();
		break;
	default:
		break;
	}

	ui_elementsList.add(ret);

	return ret;
}

void j1Gui::DestroyUIElement(char* name)
{
	p2List_item<UI_Elements*>* element = ui_elementsList.start;
	while (element != nullptr)
	{
		if (element->data->name == name) {			
			ui_elementsList.del(element);
		}
		element = element->next;
	}
}

SDL_Texture* j1Gui::GetUISpritesheet()
{
	return ui_spritesheet;
}
