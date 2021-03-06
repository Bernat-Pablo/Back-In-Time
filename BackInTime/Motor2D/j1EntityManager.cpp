#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FlyingEnemy.h"
#include "j1GroundEnemy.h"
#include "j1Coin.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"
#include "Brofiler/Brofiler.h"
#include "j1Menu.h"

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

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	
	//Entity Start() is called at CreateEntity()

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	//player->PreUpdate();

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		if (App->is_paused == true)break;
		if (entity->data->isDead == false)
			entity->data->PreUpdate();
		entity = entity->next;
	}

	BROFILER_CATEGORY("EntityM_PreUpdate", Profiler::Color::LightGreen);


	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	//player->Update(dt);
	
	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		if (App->is_paused == true)	break;
		if(entity->data->isDead == false)
			entity->data->Update(dt);
		entity = entity->next;
	}

	BROFILER_CATEGORY("EntityM_Update", Profiler::Color::Gold);

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;
	//player->PostUpdate();

	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		if (entity->data->isDead == false)
			entity->data->PostUpdate();
		entity = entity->next;
	}

	BROFILER_CATEGORY("EntityM_PostUpdate", Profiler::Color::SaddleBrown);

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		ret = entity->data->CleanUp();
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
		ret->Awake(App->GetConfig().child("entityManager").child("groundEnemy"));
		ret->Start();
		break;
	case entityTypes::FLYING_ENEMY:
		ret = new j1FlyingEnemy();
		ret->position.x = position_x;
		ret->position.y = position_y;
		ret->Awake(App->GetConfig().child("entityManager").child("flyingEnemy"));
		ret->Start();
		break;
	case entityTypes::COIN:
		ret = new j1Coin();
		ret->position.x = position_x;
		ret->position.y = position_y;
		ret->Awake(App->GetConfig().child("entityManager").child("coin"));
		ret->Start();
	case entityTypes::UNKNOWN:
		break;
	default:
		break;
	}

	if (ret)
		entitiesList.add(ret);

	if (App->menu->want_ingame_ui == true) {
		App->menu->CreateInGameUI();
		App->menu->want_ingame_ui = false;
	}	

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file)const
{
	bool ret = true;
	p2List_item<j1Entity*>* entity = entitiesList.start;
	while (entity != nullptr)
	{
		pugi::xml_node child = file.append_child(entity->data->name.GetString());
		child.append_child("position").append_attribute("x") = entity->data->position.x;
		child.child("position").append_attribute("y") = entity->data->position.y;

		entity = entity->next;
	}
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& file) 
{
	bool ret = true;
	pugi::xml_node entity_saved = file.first_child();
	//We delete all entities and create them again using saved positions
	DestroyAllEntities();

	while(entity_saved != nullptr)
	{
		p2SString entity_name(entity_saved.name());
		iPoint position_saved;
		position_saved.x = entity_saved.child("position").attribute("x").as_int();
		position_saved.y = entity_saved.child("position").attribute("y").as_int();

		if (entity_name == "flyingEnemy")
			CreateEntity(entityTypes::FLYING_ENEMY, position_saved.x, position_saved.y);
		else if (entity_name == "groundEnemy")
			CreateEntity(entityTypes::GROUND_ENEMY, position_saved.x, position_saved.y);

		entity_saved = entity_saved.next_sibling();
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
			entity->CleanUp();
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
			//If we destroy the player, sometimes the game crashes because of enemy pathfinding not finding the player
			DestroyEntity(entity->data);
		}
	}
}