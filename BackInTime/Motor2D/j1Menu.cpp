#include "j1Menu.h"
#include "j1Input.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"
#include "j1RectSprites.h"

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
	return true;
}

bool j1Menu::Update(float dt)
{
	if (menuAble) {
		App->render->DrawQuad({ 10,10,10,10 }, 100, 100, 100, 155);
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
			menuAble = !menuAble;
		}
	}
	return true;
}

bool j1Menu::CleanUp()
{
	return true;
}
