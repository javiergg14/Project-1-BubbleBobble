#include "Enemy.h"
#include "Sprite.h"

Enemy::Enemy(const Point& p, int width, int height, int frame_width, int frame_height) : 
	Entity(p, width, height, frame_width, frame_height)
{
	visibility_area = {};
	look = Look::LEFT;
}
Enemy::~Enemy()
{
}
void Enemy::DrawVisibilityArea(const Color& col) const
{
	DrawRectangleLines(visibility_area.pos.x, visibility_area.pos.y, visibility_area.width, visibility_area.height, col);
}