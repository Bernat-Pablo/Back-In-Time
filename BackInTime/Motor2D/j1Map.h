#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct SDL_Texture;
// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	float		x;
	float		y;
	float		parallax;
	uint*		data;

	MapLayer() : data(NULL){}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const {

		return x + y * width;
	}
};

struct TileSet
{


	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct ObjectGroup {
	p2SString name = "No name";
	int size = 0;
	SDL_Rect* object;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	p2List<MapLayer*>		layers;
	p2List<ObjectGroup*>	objectgroup;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint PosConverter(int x, int y);
	SDL_Rect GetTileRect(TileSet* tileset, int id);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectgroup);
	
public:

	MapData data;
	
private:

	pugi::xml_document	map_file;
	pugi::xml_document	config_file;
	p2SString			folder;
	bool				map_loaded;
	bool				finish_printing;

	p2List_item<MapLayer*>* lay = nullptr;
	MapLayer* layer;
	TileSet* tileset;
};

#endif // __j1MAP_H__