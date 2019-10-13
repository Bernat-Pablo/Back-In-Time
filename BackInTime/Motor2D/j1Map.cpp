#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include <math.h>



j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
	debug = true;
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	lay = data.layers.start;
	MapLayer* layer = lay->data;
	TileSet* tileset = data.tilesets.start->data;


	for (int l = 0; l < data.layers.count(); l++) {
		int x = 0, y = 0;
		for (int i = 0; i < layer->height; i++) {

			for (int j = 0; j < layer->width; j++) {

				int n = layer->Get(j, i);
				if (layer->data[n] != 0) {
					App->render->Blit(tileset->texture, x, y, &GetTileRect(tileset, layer->data[n]));
				}
				x += data.tile_width;
			}
			x = 0;
			y += data.tile_height;
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
	DebugDraw();
}

void j1Map::DebugDraw()
{
	if (debug)
	{
		SDL_Rect col;
		for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
			if (obj->data->name == ("Colliders"))
				for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
				{ 
					if (objdata->data->name == 1) //Terrain colliders
					{
						col.h = objdata->data->height, col.w = objdata->data->width, col.x = objdata->data->x, col.y = objdata->data->y;
						App->render->DrawQuad(col, 0, 0, 255, 100);
					}
					if (objdata->data->name == 2) //Die colliders
					{
						col.h = objdata->data->height, col.w = objdata->data->width, col.x = objdata->data->x, col.y = objdata->data->y;
						App->render->DrawQuad(col, 255, 0, 0, 100);
					}
					if (objdata->data->name == 3) //Trap colliders
					{
						col.h = objdata->data->height, col.w = objdata->data->width, col.x = objdata->data->x, col.y = objdata->data->y;
						App->render->DrawQuad(col, 255, 170, 0, 100);
					}
					if (objdata->data->name == 4) //Activate trap
					{
						col.h = objdata->data->height, col.w = objdata->data->width, col.x = objdata->data->x, col.y = objdata->data->y;
						App->render->DrawQuad(col, 255, 50, 0, 100);
					}
					if (objdata->data->name == 5) //Change scene
					{
						col.h = objdata->data->height, col.w = objdata->data->width, col.x = objdata->data->x, col.y = objdata->data->y;
						App->render->DrawQuad(col, 0, 0, 255, 100);
					}
				}
	}
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

	pugi::xml_node group;
	for (group = map_file.child("map").child("objectgroup"); group && ret; group = group.next_sibling("objectgroup"))
	{
		ObjectsGroup* set = new ObjectsGroup();

		if (ret == true)
		{
			ret = LoadObjectLayers(group, set);
		}
		data.objLayers.add(set);
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
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadObjectLayers(pugi::xml_node& node, ObjectsGroup* group)
{
	bool ret = true;

	group->name = node.attribute("name").as_string();

	for (pugi::xml_node& obj = node.child("object"); obj && ret; obj = obj.next_sibling("object"))
	{
		ObjectsData* data = new ObjectsData;

		data->height = obj.attribute("height").as_uint();
		data->width = obj.attribute("width").as_uint();
		data->x = obj.attribute("x").as_uint();
		data->y = obj.attribute("y").as_uint();
		data->name = obj.attribute("name").as_uint();

		group->objects.add(data);
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

void j1Map::ActivateDebug()
{
	if (debug == true)
		debug = false;
	else if (debug == false)
		debug = true;
}
