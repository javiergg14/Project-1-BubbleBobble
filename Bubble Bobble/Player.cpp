
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/player.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 2*n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { n, 2 * n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 2*n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i * n, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { (float)i * n, n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { (float)i * n,  n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, 3 * n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { n, 3 * n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, 3 * n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT, ANIM_DELAY_ATTACK);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT, { (float)i * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT, ANIM_DELAY_ATTACK);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT, { (float)i * n, 4 * n, -n, n });


	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
	case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
	case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
	}
}
void Player::Attack()
{
	if (IsKeyPressed(KEY_X))
	{
		state == State::ATTACKING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);
		else					SetAnimation((int)PlayerAnim::ATTACKING_LEFT);
	}
	
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();
	Attack();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				box = GetHitbox();
				
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				
				box = GetHitbox();
				box.pos.y++;
				

			}
			else if (IsKeyPressed(KEY_Z))
			{
				StartJumping();
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
}
void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}