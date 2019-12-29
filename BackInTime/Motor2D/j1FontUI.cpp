#include "j1FontUI.h"
#include "j1Player.h"
#include "j1Fonts.h"
#include "Brofiler/Brofiler.h"

Font_UI::Font_UI()
{
	text = nullptr;
}

bool Font_UI::Start()
{
	text = this->text_font;
	position = this->pos;
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

	BROFILER_CATEGORY("FontUI_Update", Profiler::Color::Aquamarine);
	return true;
}

bool Font_UI::CleanUp()
{
	return true;
}
