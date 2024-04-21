#include "Balas.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Game.h"
#include <raymath.h>

Balas :: Balas(const Point& p, BalaState s, BalaLook view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
}
Balas::~Balas()
{
}
void Balas::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Balas::BalasTest(Point playerPos)
{
	TraceLog(LOG_INFO, "Entra");
	state = BalaState::LANZAR;
	SetAnimation((int)BalasAnim::SHOT);
	SetPos(playerPos);
}
void Balas::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
AppStatus Balas::Initialise()
{

	float n = 16;
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_BULLET, "images/shoot.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_BULLET));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BalasAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BalasAnim::SHOT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BalasAnim::SHOT, { 0, 0, n, n });

	return AppStatus::OK;
}
void Balas::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Balas::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.

	
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Balas::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BULLET);

	render->Release();
}

