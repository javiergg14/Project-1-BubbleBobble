#include "ShotManager.h"

ShotManager::ShotManager()
{
	map = nullptr;
	particles = nullptr;
}
ShotManager::~ShotManager()
{
}
AppStatus ShotManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::SHOOT, "images/Shoot.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void ShotManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void ShotManager::SetParticleManager(ParticleManager* particles)
{
	this->particles = particles;
}
void ShotManager::Add(const Point& pos, const Point& dir)
{
	bool found = false;
	for (Shot& shot : shots)
	{
		shot.Init(pos, dir);
	}
	if(!found) LOG("Not enough space to add a new shot in the ShotManager!");
}
void ShotManager::Clear()
{
	for (Shot& shot : shots)
		shot.SetAlive(false);
}
void ShotManager::Update(const AABB& player_hitbox)
{
	AABB box;
	bool hit;
	
	for (Shot& shot : shots)
	{
		if (shot.IsAlive())
		{
			//Update position
			shot.Update();
			
			//Check level collisions
			box = shot.GetHitbox();
			hit = false;
			if(shot.IsMovingLeft())		  hit = map->TestCollisionWallLeft(box);
			else if(shot.IsMovingRight()) hit = map->TestCollisionWallRight(box);

			//Check player collision
			if(!hit) hit = box.TestAABB(player_hitbox);

			if(hit)
			{
				shot.SetAlive(false);
				Point p;
				p.x = box.pos.x - (TILE_SIZE-SHOT_PHYSICAL_WIDTH)/2;
				p.y = box.pos.y - (TILE_SIZE-SHOT_PHYSICAL_HEIGHT)/2;
				particles->Add(p);
			}
		}
	}
}
void ShotManager::Draw() const
{
	//Iterate over the shots vector by reference instead of by value to avoid a copy
	//of each Entity object
	for (const Shot& shot : shots)
		if (shot.IsAlive())	shot.Draw();
}
void ShotManager::DrawDebug(const Color& col) const
{
	for (const Shot& shot : shots)
		if (shot.IsAlive()) shot.DrawHitbox(col);
}
