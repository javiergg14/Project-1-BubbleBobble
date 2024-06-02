#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
	img_tiles1 = nullptr;
	img_tiles2 = nullptr;
	img_tiles3 = nullptr;
	img_tiles4 = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK] = { 0,  0, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE] = { n,  0, n, n };	
	dict_rect[(int)Tile::SHADOW] = { 2 * n, 0, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE_VERTICAL] = { 3 * n, 0, n, n };
	dict_rect[(int)Tile::SHADOW_VERTICAL_EDGE_2] = { 2 * n,  n, n, n };
	dict_rect[(int)Tile::VERTICAL_SHADOW] = { n,  n, n, n };
	dict_rect[(int)Tile::TOTAL_EDGE_SHADOW] = { 0, n, n, n };
	dict_rect[(int)Tile::BLOQUE2] = { 0, 2*n, n, n };
	dict_rect[(int)Tile::LATERAL21] = { 4*n, 2*n, n, n };
	dict_rect[(int)Tile::LATERAL22] = { 5 * n, 2 * n, 2 * n, 2 * n };
	dict_rect[(int)Tile::LATERAL23] = { 4 * n, 3 * n, 2 * n, 2 * n };
	dict_rect[(int)Tile::LATERAL24] = { 5 * n, 3 * n, 2 * n, 2 * n };
	dict_rect[(int)Tile::BLOQUE3] = { 0, 4*n, n, n };
	dict_rect[(int)Tile::LATERAL32] = { 4*n, 5*n, n, n };
	dict_rect[(int)Tile::LATERAL33] = { 5 * n, 5 * n, n, n };
	dict_rect[(int)Tile::ESQUINA4] = { 0, n, n, n };
	dict_rect[(int)Tile::BLOQUE4] = { 0, n, n, n };
	dict_rect[(int)Tile::LATERAL4] = { 0, n, n, n };
	dict_rect[(int)Tile::HORIZONTALESQUINA4] = { 0, n, n, n };
	dict_rect[(int)Tile::HORIZONTAL4] = { 0, n, n, n };
	dict_rect[(int)Tile::HORIZONTALESQUINA4] = { 0, n, n, n };
	dict_rect[(int)Tile::VERTICAL4] = { 0, n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES1, "images/Tilesetniveles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles1 = data.GetTexture(Resource::IMG_TILES1);

	if (data.LoadTexture(Resource::IMG_FRUITS, "images/Fruits.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_items = data.GetTexture(Resource::IMG_FRUITS);

	laser = new Sprite(img_tiles1);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles1, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES1);

	laser->Release();

	dict_rect.clear();
}