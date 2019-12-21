#ifndef _j1COIN_H
#define _j1COIN_H

#include "j1Entity.h"

class j1Coin : public j1Entity
{
public:
	j1Coin();
	~j1Coin();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
};

#endif // !_j1COIN_H