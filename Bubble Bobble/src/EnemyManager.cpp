#include "EnemyManager.h"
#include "Zen-Chan.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/zen-chan.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}
	else if (data.LoadTexture(Resource::PART, "images/puntuaciones.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy *enemy;
	
	if (type == EnemyType::SLIME)
	{
		enemy = new Slime(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE);
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}
		
	enemy->Initialise(look, area);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SLIME)
	{
		width = SLIME_PHYSICAL_WIDTH;
		height = SLIME_PHYSICAL_HEIGHT;
	}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	bool shoot;
	Point p, d;

	for (Enemy* enemy : enemies)
	{
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}
	}
}
std::vector<Enemy*> EnemyManager:: GetEnemies() const
{
	return enemies;
}
std::vector<AABB> EnemyManager::GetHitBoxes() const
{
	std::vector<AABB> hitboxes;
	for (Enemy* enemy : enemies)
	{
		hitboxes.push_back(enemy->GetHitbox());
	}

	return hitboxes;
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{	
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}