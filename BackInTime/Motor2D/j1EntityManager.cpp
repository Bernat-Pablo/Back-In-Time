#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager()
{

}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;


	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;

	p2List_item<j1Entity*>* tmp = entitiesList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
		
	p2List_item<j1Entity*>* tmp = entitiesList.start;
	while(tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;

	p2List_item<j1Entity*>* tmp = entitiesList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* tmp = entitiesList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->CleanUp();
		tmp = tmp->next;
	}

	return ret;
}

j1Entity* CreateEntity(entityTypes type, int position_x, int position_y)
{
	j1Entity* ret = nullptr;

	switch(type)
	{
	case entityTypes::PLAYER:
		ret = new j1Player();
		ret->position.x = position_x;
		ret->position.y = position_y;
		break;
	//case entityTypes::WALKING_ENEMY:
	//	ret = new j1WalkingEnemy();
	//	ret->position.x = position_x;
	//	ret->position.y = position_y;
	//	break;
	//case entityTypes::FLYING_ENEMY:
	//	ret = new j1FlyingEnemy();
	//	ret->position.x = position_x;
	//	ret->position.y = position_y;
	//	break;
	case entityTypes::UNKNOWN:
		break;
	default:
		break;
	}

	return ret;
}