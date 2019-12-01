#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Player.h"
#include "Brofiler/Brofiler.h"



j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	App->collision->AddCollider({ 0,0,1,1000 }, COLLIDER_WALL, "map");
	folder.create(config.child("folder").child_value());

	pugi::xml_parse_result result = config_file.load_file("config.xml");
	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", result, result.description());
		ret = false;
	}

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	lay = data.layers.start;
	layer = lay->data;
	tileset = data.tilesets.start->data;
	pugi::xml_node node = config_file.child("config").child("map");

	for (int l = 0; l < data.layers.count(); l++) {
		finish_printing = false;
		int x = 0, y = 0;
		for (int i = 0; i < layer->height; i++) {

			for (int j = 0; j < layer->width && finish_printing == false; j++) {

				int n = layer->Get(j, i);
				if (lay->data->name == "Colliders_2") {
					continue;
				}
				if (lay->data->name == "Arboles P") {
					//culling left and up with parallax
					if (layer->data[n] != 0 && x >= (App->player->position.x - node.child("finish_printing_left").attribute("value").as_int() * 16) * 0.9 && (App->player->position.y - node.child("finish_printing_up").attribute("value").as_int() * 16)) {
						App->render->Blit(tileset->texture, x, y, &GetTileRect(tileset, layer->data[n]), 0.9f);
					}
				}
				if (lay->data->name == "Arboles2 P") {
					//culling left and up with parallax
					if (layer->data[n] != 0 && x >= (App->player->position.x - node.child("finish_printing_left").attribute("value").as_int() * 16) * 0.9 && (App->player->position.y - node.child("finish_printing_up").attribute("value").as_int() * 16)) {
						App->render->Blit(tileset->texture, x, y, &GetTileRect(tileset, layer->data[n]), 0.9f);
					}
				}
				else
					//culling left without parallax
					if (layer->data[n] != 0 && x >= (App->player->position.x - node.child("finish_printing_left").attribute("value").as_int() * 16) && (App->player->position.y- node.child("finish_printing_up").attribute("value").as_int() * 16))
						App->render->Blit(tileset->texture, x, y, &GetTileRect(tileset, layer->data[n]));
				x += data.tile_width;
				//culling right
				if (x >= node.child("finish_printing_right").attribute("value").as_int() * 16 + App->player->position.x) {
					j = layer->width;
				}
			}
			x = 0;
			y += data.tile_height;
			//culling down
			if (y >= node.child("finish_printing_down").attribute("value").as_int() * 16 + App->player->position.y) {
				i = layer->height;
			}
			}
		if (lay->next != nullptr) 
		{
			if (lay->next->data->name != "Collider") 
			{
				lay = lay->next;
				layer = lay->data;
			}
			else 
			{
				l++;
			}
					
		}
	}	
	BROFILER_CATEGORY("Map_Render", Profiler::Color::AntiqueWhite);
}

iPoint j1Map::PosConverter(int x, int y) {
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect j1Map::GetTileRect(TileSet* tileset,int id) 
{
	int num = id;
	int x = (num - tileset->firstgid) % tileset->num_tiles_width;
	int y = (num - tileset->firstgid) / tileset->num_tiles_width;
	int width = x * data.tile_width + x * tileset->margin;
	int height = y * data.tile_height + y * tileset->spacing;
	SDL_Rect rect = { width,height,tileset->tile_width,tileset->tile_height };

	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	p2List_item<ObjectGroup*>* item3;
	item3 = data.objectgroup.start;
	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.objectgroup.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(file_name);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}

	//Load objectgroup info
	pugi::xml_node group;
	for (group = map_file.child("map").child("objectgroup"); group && ret; group = group.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroup(group, set);
		}
		data.objectgroup.add(set);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* item_object = data.objectgroup.start;

		while (item_object != NULL)
		{
			ObjectGroup* o = item_object->data;
			LOG("ObjectGroup ----");
			LOG("name: %s", o->name.GetString());
			item_object = item_object->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectgroup)
{
	bool ret = true;

	pugi::xml_node object = node.child("object");
	SDL_Rect rect = { 0,0,0,0 }; 
	uint i = 0u;
	uint collidernum = 0; //number of colliders
	if (object == NULL)
	{
		LOG("Error loading object group");
		ret = false;
	}else 
	{
		objectgroup->object = new SDL_Rect[MAX_COLLIDERS];

		while (object != NULL)
		{				
			p2SString name(object.attribute("name").as_string());
			
			if(name != NULL)
			{
				objectgroup->object[i].x = object.attribute("x").as_int();
				objectgroup->object[i].y = object.attribute("y").as_int();
				objectgroup->object[i].w = object.attribute("width").as_int();
				objectgroup->object[i].h = object.attribute("height").as_int();				

				if(name == "1")
					App->collision->AddCollider(objectgroup->object[i], COLLIDER_WALL, "wall");
				if(name == "2")
					App->collision->AddCollider(objectgroup->object[i],  COLLIDER_DIE, "die");
				if (name == "5")
					App->collision->AddCollider(objectgroup->object[i], COLLIDER_DOOR, "door1");
				if (name == "6")
					App->collision->AddCollider(objectgroup->object[i], COLLIDER_DOOR, "door2");
				if (name == "7") //Spawn flying enemy
				{					 
					j1Entity* flyingEnemy1 = App->entityManager->CreateEntity(entityTypes::FLYING_ENEMY, objectgroup->object[i].x, objectgroup->object[i].y);
					flyingEnemy1->Awake(App->GetConfig().child("entityManager").child("flyingEnemy"));
				}	
				if (name == "8") //Spawn ground enemy
				{
					//j1Entity* groundEnemy1 = App->entityManager->CreateEntity(entityTypes::GROUND_ENEMY , objectgroup->object[i].x, objectgroup->object[i].y);
					
					//groundEnemy1->Awake(App->GetConfig());
					//groundEnemy1->Start();
				}
				if (name == "9") //Spawn player
				{
					//App->player = new j1Player();
					//App->AddModule(App->player);
				}

				collidernum++;
			}			

			object = object.next_sibling("object");

			//LOG("Collider %i", i);
			//LOG("Total colliders is: %i", collidernum);

			i++;
		}
	}


	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			if (tile.attribute("gid").as_int() == NULL) {
				layer->data[i++] = 0;
			}
			else {
				layer->data[i++] = tile.attribute("gid").as_int();
			}
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);
		int p = 0;
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tile_id = layer->Get(x, y);
				

				if (layer->data[tile_id] > 0)
				{
					map[i] = 0; //here we have to put our tile number and set it 0 navigation.
					p++;
				}
			}
		}
		LOG("%i", p);
		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}
