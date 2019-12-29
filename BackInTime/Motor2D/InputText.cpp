#include "InputText.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Console.h"

bool InputText::Start()
{
	position = this->pos;
	r = this->rect;
	spacing = 2;
	line.x = position.x + spacing;
	line.y = position.y + spacing;
	line.w = 1;
	line.h = 16;
	number_letters = 0;
	ppos.x = line.x-8;
	ppos.y = line.y + 3;
	return true;
}

bool InputText::Update(float dt)
{
	App->render->DrawQuad(rect, 50, 0, 0, 100);
	
	tick1 = SDL_GetTicks();
	line.x = position.x + spacing + 8 * number_letters;

	if (tick1 - tick2 >= 1000) {
		App->render->DrawQuad(line, 0, 0, 0, 255);
		tick2 = SDL_GetTicks();
	}
	
	tick3 = SDL_GetTicks();
	if (clicked) {
		text = App->input->text_frominput.GetString();
		App->fonts->BlitText(ppos.x,line.y,1,text);
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
