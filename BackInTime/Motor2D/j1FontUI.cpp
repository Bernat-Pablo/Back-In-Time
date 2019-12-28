#include "j1FontUI.h"
#include "j1Player.h"
#include "j1Fonts.h"

Font_UI::Font_UI()
{
}

bool Font_UI::Start()
{
	text = this->text_font;
	return true;
}

bool Font_UI::Update(float dt)
{
	if (this->following_pj) {
		position.x = App->player->position.x;
		position.y = App->player->position.y;
	}
	else {
		position = this->pos;
	}

	App->fonts->BlitText(position.x, position.y, 1, text);
	return true;
}

bool Font_UI::CleanUp()
{
	return true;
}
