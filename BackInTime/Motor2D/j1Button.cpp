#include "j1Button.h"
#include "j1App.h"
#include "p2Point.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Fonts.h"

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

	App->render->DrawQuad(r, 0, 255, 0, 255);
	App->fonts->BlitText(r.x + this->margeButton.x, r.y + this->margeButton.y, 1, text);

	return true;
}

bool Button::CleanUp()
{
	return true;
}

void Button::Draw()
{
}

void Button::onClick()
{
}
