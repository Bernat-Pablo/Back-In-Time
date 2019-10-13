#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	float		x;
	float		y;
	uint*		data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const {

		return x + y * width;
	}
};

struct ObjectsData
{
	uint16_t	name;
	int			x;
	int			y;
	uint		width;
	uint		height;

};

struct ObjectsGroup
{
	p2SString				name;
	p2List<ObjectsData*>	objects;
	~ObjectsGroup();
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
	p2List<ObjectsGroup*>	objLayers;
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

	//Activates/Deactivates debug mode
	void ActivateDebug(); 
	void DebugDraw();
private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectLayers(pugi::xml_node& node, ObjectsGroup* group);
	
public:

	MapData data;
	
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

	bool debug; //If true, we draw debug objects (colliders)

	p2List_item<MapLayer*>* lay = nullptr;
};

#endif // __j1MAP_H__