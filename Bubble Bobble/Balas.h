#pragma once
#include "Entity.h"
#include "TileMap.h"

enum class BalaState {INVISIBLE, LANZAR, CONTACT };
enum class BalaLook { RIGHT, LEFT };

enum class BalasAnim {
	SHOT, NUM_ANIMATIONS
};

class Balas : public Entity {
public:

	Balas(const Point& p, BalaState s, BalaLook view);
	~Balas();
	void SetTileMap(TileMap* tilemap);
	AppStatus Initialise();
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	void SetAnimation(int id);
	void BalasTest();
private:
	BalaState state;
	BalaLook look;
	TileMap* map;
};
