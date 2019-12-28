#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Image.h"
#include "j1Textures.h"
#include "j1FontUI.h"
#include "j1Button.h"

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
		element->data->PreUpdate();
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
		element->data->Update(dt);
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
		element->data->PostUpdate();
		element = element->next;
	}

	return ret;
}

bool j1Gui::CleanUp()
{
	bool ret = true;

	ui_spritesheet = nullptr;

	return ret;
}

UI_Elements* j1Gui::CreateUIElement(UI_Types type, int position_x, int position_y, char* texture, bool follow_pj, char* t)
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
		ret->Start();
		break;
	case UI_Types::BUTTON:
		ret = new Button();
		ret->rect = { position_x ,position_y, 80,30 };
		ret->text_font = t;
		ret->Start();
		break;
	default:
		break;
	}

	ui_elementsList.add(ret);

	return ret;
}

SDL_Texture* j1Gui::GetUISpritesheet()
{
	return ui_spritesheet;
}
