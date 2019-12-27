#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Image.h"
#include "j1Textures.h"

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

UI_Elements* j1Gui::CreateUIElement(UI_Types type, int position_x, int position_y)
{
	UI_Elements* ret = nullptr;

	switch (type)
	{
	case UI_Types::TEXT:
		ret = new j1Fonts();		
		break;
	case UI_Types::IMAGE:
		ret = new j1Image();
		ret->Start();
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
