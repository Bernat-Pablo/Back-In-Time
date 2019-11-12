#ifndef __RectSprites_H__
#define __RectSprites_H__
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

class RectSprites {

public:
	SDL_Rect rect;
	iPoint offset;
	iPoint offset_reverse;
	iPoint displacement;
	SDL_Rect hurtColliders[3];
	SDL_Rect hitCollider;


public:
	RectSprites() {};
	RectSprites(SDL_Rect rect, iPoint point, iPoint displ = { 0,0 }) : rect(rect), offset(point), displacement(displ) {}
	void SetOffset(int x, int y) {
		offset.x += x;
		offset.y += y;
	}

	void SetDisplacement(int x, int y)
	{
		displacement.x += x;
		displacement.y += y;
	}

};

#endif // !__RectSprites_H__