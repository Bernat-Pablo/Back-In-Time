#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class UI_Elements;
class j1Image;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void startfromBeginning();

	void CreateUI();
	void UpdateUI();

public:
	float fade_time;

	int choose_lv = 1;

	p2List<j1Image*> screen_ui;
private:
	SDL_Texture* debug_tex;
};

#endif // __j1SCENE_H__