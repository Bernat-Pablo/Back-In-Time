#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FlyingEnemy.h"
#include "j1GroundEnemy.h"
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
	player = new j1Player();
	player->Awake(config.child("player"));	
	entitiesList.add(player);
	

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	player->Start();


	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	player->PreUpdate();

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	player->Update(dt);

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;
	player->PostUpdate();

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

j1Entity* j1EntityManager::CreateEntity(entityTypes type, int position_x, int position_y)
{
	j1Entity* ret = nullptr;

	switch(type)
	{
	case entityTypes::PLAYER:
		ret = new j1Player();
		//ret->position.x = position_x;
		//ret->position.y = position_y;
		break;
	case entityTypes::GROUND_ENEMY:
		ret = new j1GroundEnemy();
		ret->position.x = position_x;
		ret->position.y = position_y;
		break;
	case entityTypes::FLYING_ENEMY:
		ret = new j1FlyingEnemy();
		ret->position.x = position_x;
		ret->position.y = position_y;
		break;
	case entityTypes::UNKNOWN:
		break;
	default:
		break;
	}

	if (ret)
		entitiesList.add(ret);

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file)const
{
	bool ret = true;
	p2List_item<j1Entity*>* tmp = entitiesList.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	return ret;
}