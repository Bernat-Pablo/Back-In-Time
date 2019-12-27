#ifndef _J1_IMAGE_H
#define _J1_IMAGE_H

#include "p2Point.h"
#include "UI_Elements.h"
#include "SDL_image/include/SDL_image.h"

class j1Image : public UI_Elements
{
public:
	j1Image();
	~j1Image() {};

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void Draw();

private:
	SDL_Texture* texture;
	iPoint position;
};

#endif // !_J1_IMAGE_H
