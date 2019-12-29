#include "j1Console.h"
#include "j1Gui.h"
#include "UI_Elements.h"
#include "j1Input.h"
#include "p2SString.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"

j1Console::j1Console()
{
	name.create("console");
	god = "god_mode";
	quit = "quit";
	fps = "fps";
	map = "map";
	list = "god_mode,quit,fps<>,map<>";
}

bool j1Console::Awake()
{
	return true;
}

bool j1Console::Start()
{
	return true;
}

bool j1Console::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Console_PreUpdate", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		open = !open;
	}
	if (open) {
		App->render->DrawQuad({ 200,0,200,200 }, 0, 0, 255, 255);
		App->gui->CreateUIElement(UI_Types::INPUTTEXT, 200, 180, "console", "0", false, "0", 200);
		App->is_paused = true;
	}
	else {
		App->gui->DestroyUIElement("console");
		App->is_paused = false;
	}
	BROFILER_CATEGORY("Console_Update", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Console_PostUpdate", Profiler::Color::Aquamarine);
	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

