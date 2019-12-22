#include "j1EntityManager.h"
#include "j1Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Fade.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"

j1Entity::j1Entity(entityTypes type) : j1EntityManager()
{
	spritesheet_entity = nullptr;
	collider_entity = nullptr;

	isDead = false;
	moving_right = false;
	moving_left = false;
	isgrounded = false;
	being_hit = false;
	reversed = false;

	collider_at_right = false; //If true, entity can't go to the right 
	collider_at_left = false;
}

j1Entity::~j1Entity()
{
}