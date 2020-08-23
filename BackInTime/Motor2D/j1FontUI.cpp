#include "j1FontUI.h"
#include "j1Player.h"
#include "j1Fonts.h"
#include "Brofiler/Brofiler.h"
#include "j1Render.h"

Font_UI::Font_UI()
{
	text = nullptr;
}

bool Font_UI::Start()
{
	text = this->text_font;
	position = local_position = this->pos;
	return true;
}

bool Font_UI::Update(float dt)
{
	if (this->following_pj) {
		//position.x = -(0.5 * App->render->camera.x) + local_position.x;
		//position.y = -(0.5 * App->render->camera.y) + local_position.y;

		//position.x = App->render->camera.x;
		//position.y = App->render->camera.y;

		position.x = App->player->position.x;
		position.y = App->player->position.y;
		LOG("position x: %i", position.x);
		LOG("position y: %i", position.y);
		LOG("local position x: %i", local_position.x);
		LOG("local position y: %i", local_position.y);
		LOG("Player x: %i", App->player->position.x);
		LOG("Player y: %i", App->player->position.y);
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
