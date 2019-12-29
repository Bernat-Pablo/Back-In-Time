#include "InputText.h"
#include "j1App.h"
#include "j1Render.h"

bool InputText::Start()
{
	position = this->pos;
	r = this->rect;
	spacing = 2;
	line.x = position.x + spacing;
	line.y = position.y + spacing;
	line.w = 1;
	line.h = 16;
	return true;
}

bool InputText::Update(float dt)
{
	App->render->DrawQuad(rect, 50, 0, 0, 100);
	tick1 = SDL_GetTicks();
	if (tick1 - tick2 >= 1000) {
		App->render->DrawQuad(line, 0, 0, 0, 255);
		tick2 = SDL_GetTicks();
	}
	
	return true;
}

bool InputText::PostUpdate()
{
	return true;
}

bool InputText::CleanUp()
{
	return true;
}

bool InputText::OnClick()
{
	return true;
}
