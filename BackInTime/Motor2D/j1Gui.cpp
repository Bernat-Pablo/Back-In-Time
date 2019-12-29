#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Image.h"
#include "j1Textures.h"
#include "j1FontUI.h"
#include "j1Button.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"
#include "Slider.h"
#include "InputText.h"

j1Gui::j1Gui()
{
	name.create("gui");
	ui_spritesheet = nullptr;
	changing = false;
}

bool j1Gui::Awake(pugi::xml_node& config)
{
	bool ret = true;

	ui_spritesheet_path = config.child("texture").attribute("path").as_string("");
	
	return ret;
}

bool j1Gui::Start()
{
	bool ret = true;

	ret = true;
	ui_spritesheet = App->tex->Load(ui_spritesheet_path.GetString());
	App->fonts->Load("fonts/small_white_font.png", " abcdefghiklmnoprstuwy'.0123456789", 1, 8,8,34);
	
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
	BROFILER_CATEGORY("GUI_PreUpdate", Profiler::Color::Aquamarine);
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
	BROFILER_CATEGORY("GUI_Update", Profiler::Color::Aquamarine);
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
	BROFILER_CATEGORY("GUI_PostUpdate", Profiler::Color::Aquamarine);
	return ret;
}

bool j1Gui::CleanUp()
{
	bool ret = true;

	ui_spritesheet = nullptr;

	return ret;
}

UI_Elements* j1Gui::CreateUIElement(UI_Types type, int position_x, int position_y,char* name, char* texture, bool follow_pj, char* t, int w, int h)
{
	UI_Elements* ret = nullptr;

	switch (type)
	{
	case UI_Types::TEXT:
		ret = new Font_UI();
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
		ret->following_pj = follow_pj;
		ret->margeButton.x = -ret->numberLetters*4+40;
		ret->rect = { position_x ,position_y, 80,30 };
		ret->text_font = t;
		ret->name = name;
		ret->Start();
		break;
	case UI_Types::SLIDER:
		ret = new Slider();
		ret->pos.x = position_x;
		ret->pos.y = position_y;
		ret->following_pj = follow_pj;
		ret->name = name;
		ret->Start();
		break;
	case UI_Types::INPUTTEXT:
		ret = new InputText();
		ret->pos.x = position_x;
		ret->pos.y = position_y;
		ret->following_pj = follow_pj;
		ret->name = name;
		ret->rect = { position_x,position_y,w,20 };
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
		if (element->data->name == name) 		
			ui_elementsList.del(element);
		
		element = element->next;
	}
}

SDL_Texture* j1Gui::GetUISpritesheet()
{
	return ui_spritesheet;
}
