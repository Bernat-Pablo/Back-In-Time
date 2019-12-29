#include "j1Button.h"
#include "j1App.h"
#include "p2Point.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "p2Log.h"

Button::Button()
{
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

	if(On())
		App->render->DrawQuad(r, 0, 155, 0, 255);
	else App->render->DrawQuad(r, 0, 255, 0, 255);

	App->fonts->BlitText(r.x + this->margeButton.x, r.y + this->margeButton.y, 1, text);
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
			LOG("BUTTON", "s");
			return true;
		}
	}
	return false;
}

bool Button::OnClick()
{
	return false;
}
