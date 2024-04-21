#pragma once
#include "Entity.h"
#include "TileMap.h"

enum class BalaState {INVISIBLE, LANZAR, CONTACT, DERECHA, IZQUIERDA};
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
	void BalasDerecha(Point playerPos);
	void BalasIzquierda(Point playerPos);
private:
	BalaState state;
	BalaLook look;
	TileMap* map;
	int lifeTimer = 10;
	const int maxLifeTime = 1;
};
