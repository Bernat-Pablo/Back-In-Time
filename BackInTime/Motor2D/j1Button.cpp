#include "j1Button.h"
#include "j1App.h"
#include "p2Point.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Menu.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Fade.h"
#include "Brofiler/Brofiler.h"

Button::Button()
{
	credits_timer = 0;
	credits_opened = false;
	text = nullptr;
}

bool Button::Start()
{
	text = this->text_font;
	r = this->rect;

	return true;
}

bool Button::Update(float dt)
{
	if (this->following_pj) {
		r.x = App->player->position.x;
		r.y = App->player->position.y; 
	}
	text = this->text_font;

	if(On())App->render->DrawQuad(r, 0, 155, 0, 255);
	else App->render->DrawQuad(r, 0, 255, 0, 255);

	tick1 = SDL_GetTicks();

	if(OnClick())
		App->render->DrawQuad(r, 0, 55, 0, 255);

	App->fonts->BlitText(r.x + this->margeButton.x, r.y + this->margeButton.y, 1, text);

	if(credits_opened)credits_timer += dt;
	if (credits_timer >= 5) 
	{
		credits_opened = false; 
		credits_timer = 0;
	}

	BROFILER_CATEGORY("Button_PreUpdate", Profiler::Color::Aquamarine);
	return true;
}
bool Button::PostUpdate() {
	if (OnClick() && tick1 - tick2 >= 500) {
		if (this->name == "play") {
			App->fade->FadeToBlack(App->gui, App->scene);
			App->menu->menuState = INGAME_UI;
			App->gui->changing = true;
			App->menu->CleanUp();
		}
		if (this->name == "continue")
		{
			App->player->continue_button = true;
			App->menu->menuState = INGAME_UI;
			App->gui->changing = true;
			App->LoadGame();
			App->menu->CleanUp();
		}
		if(this->name == "settings")
		{
			if (App->menu->config) {
				App->gui->DestroyUIElement("music");
				App->gui->DestroyUIElement("fx");
				App->gui->DestroyUIElement("music_text");
				App->gui->DestroyUIElement("fx_text");
				App->menu->config = false;
			}
			else {
				App->gui->CreateUIElement(UI_Types::SLIDER, 100, 250, "music");
				App->gui->CreateUIElement(UI_Types::SLIDER, 200, 250, "fx");
				App->gui->CreateUIElement(UI_Types::TEXT, 80, 220, "music_text","0",false,"music");
				App->gui->CreateUIElement(UI_Types::TEXT, 172, 220, "fx_text", "0", false, "effects");
				
				App->menu->config = true;
			}
		}
		else if (this->name == "credits")
		{
			if (credits_opened == false)
			{
				ShellExecute(NULL, "open", "https://bernat-pablo.github.io/Back-In-Time/", NULL, NULL, SW_SHOWNORMAL);
				credits_opened = true;
			}
		}
		else if (this->name == "out") return false;	
		tick2 = SDL_GetTicks();
	}

	return true;
}

bool Button::CleanUp()
{

	return true;
}


bool Button::On()
{
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (mouse.x<r.x + r.w && mouse.x>r.x) {
		if (mouse.y<r.y + r.h && mouse.y>r.y) {
			return true;
		}
	}
	return false;
}

bool Button::OnClick()
{
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (mouse.x<r.x + r.w && mouse.x>r.x) {
		if (mouse.y<r.y + r.h && mouse.y>r.y) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
				return true;
			}
		}
	}
	return false;
}
