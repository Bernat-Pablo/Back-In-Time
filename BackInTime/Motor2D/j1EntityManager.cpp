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
	//Spawn player
	//player = CreateEntity(entityTypes::PLAYER, 0, 0);
	//player->Awake(config.child("player"));	
	//entitiesList.add(player);
	
	//Spawn test enemy
	/*testFlyingEnemy = CreateEntity(entityTypes::FLYING_ENEMY, 100, 0);	
	testFlyingEnemy->Awake(config.child("flyingEnemy"));	
	entitiesList.add(testFlyingEnemy);*/

	/*testFlyingEnemy2 = CreateEntity(entityTypes::FLYING_ENEMY, 200, 0);
	testFlyingEnemy2->Awake(config.child("flyingEnemy"));
	entitiesList.add(testFlyingEnemy2);*/

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	//player->Start();

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		entity->data->Start();
		entity = entity->next;
	}

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	//player->PreUpdate();

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		entity->data->PreUpdate();
		entity = entity->next;
	}
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	//player->Update(dt);
	
	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		entity->data->Update(dt);
		entity = entity->next;
	}
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;
	//player->PostUpdate();

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		entity->data->PostUpdate();
		entity = entity->next;
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		//ret = entity->data->CleanUp();
		//entity->data->DestroyEntity(entity->data);
		entity = entity->next;
	}	
	DestroyAllEntities();

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

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	p2List_item<j1Entity*>* item;

	if (entity != nullptr) {
		item = entitiesList.At(entitiesList.find(entity));
		if (entity->collider_entity != nullptr)
		{
			delete entity->collider_entity;
			entity->collider_entity = nullptr;
		}
		entitiesList.del(item);;
	}
}

void j1EntityManager::DestroyAllEntities()
{
	p2List_item<j1Entity*>* entity;

	for (entity = entitiesList.start; entity != nullptr; entity = entity->next)
	{
		if (entity->data != player) {
			DestroyEntity(entity->data);
		}
	}
}