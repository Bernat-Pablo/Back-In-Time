#include "j1Console.h"

j1Console::j1Console()
{
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
	return true;
}

bool j1Console::Update(float dt)
{
	return true;
}

bool j1Console::PostUpdate(float dt)
{
	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

