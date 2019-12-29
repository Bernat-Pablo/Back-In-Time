#ifndef J1_CONSOLE_H
#define J1_CONSOLE_H

#include "j1App.h"
#include "j1Module.h"
#include "p2SString.h"

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console() {};

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

private:
	bool open = false;
	p2SString god, quit, fps, map, list;
};
#endif // !J1_CONSOLE_H
