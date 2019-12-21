#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Fonts.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}


// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UI_Element*>* element = ui_list.start;
	while (element != nullptr) {
		

		element->data->fontModule->BlitText(element->data->position.x, element->data->position.y, 0, "hola");
	}


	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

UI_Element* j1Gui::CreateUI_Element(UI_TypeElement type, int position_x, int position_y)
{
	UI_Element* ret = nullptr;

	if (type == UI_TypeElement::TEXT) {
		ret = new j1Fonts();
		ret->position.x = position_x;
		ret->position.y = position_y;
	}

	if(ret != nullptr)
		ui_list.add(ret);

	return ret;
}

void j1Gui::destroyUI_Element(j1Gui * element)
{
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

