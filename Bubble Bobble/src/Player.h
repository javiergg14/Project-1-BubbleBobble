#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "ShotManager.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	26
#define PLAYER_PHYSICAL_HEIGHT	30

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		13

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

#define PLAYER_SPAWN {80, 430}

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

#define SLIME_SHOT_OFFSET_X_LEFT	-14
#define SLIME_SHOT_OFFSET_X_RIGHT	 26
#define SLIME_SHOT_OFFSET_Y			-22

#define SLIME_SHOT_SPEED	4

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING, DEAD, ATTACKING };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	ATTACKING_RIGHT,ATTACKING_LEFT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS,DEAD_LEFT, DEAD_RIGHT,
};
class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void SetEnemiesHitbox(std::vector<AABB>hitboxes);

	void InitVida();
	void IncrVida(int n);
	int GetVida();
	int CheckVida();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void GetShootingPosDir(Point* pos, Point* dir) const;
	void SetShotManager(ShotManager* shots);

	void StartDeath();
	void LogicDead();


private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();

	int attack_delay;

	//Animation management
	void SetAnimation(int id);
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void Enemy();

	PlayerAnim GetAnimation();

	//Jump steps
	bool IsAscending() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;

	TileMap* map;

	int score, vida;

	ShotManager* shots;

	std::vector<AABB> enemies_hitbox;



	Sound JumpSound;
	Sound AttackSound;

	int v = 0;

};

