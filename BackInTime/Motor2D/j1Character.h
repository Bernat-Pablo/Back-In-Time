#ifndef _j1CHARACTER_H_
#define _j1CHARACTER_H_

#include "j1Character.h"
#include "j1App.h"
#include "j1Animation.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"




class j1Character
{
public:
	j1Character() {};
	~j1Character() {};
	virtual bool Start()=0;
	virtual bool CleanUp()=0;

public:
	
	//BASIC MOVES
	Animation idle;
	Animation walk;
	Animation death;
	Animation hurt;
	Animation jump;
	Animation run;
	Animation throw_rock;
	Animation walk_atack;
	Animation atack1;
	Animation atack2;

	SDL_Texture* texture;


};



#endif // !_CHARACTER_H_
