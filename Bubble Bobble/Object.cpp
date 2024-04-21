#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;

	Rectangle rc;
	const int n = 32;
	switch (type)
	{
	case ObjectType::MORADO: rc = { 0, 0, n, n }; break;
	case ObjectType::ROJO: rc = { 4*n, 0, n, n }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_ENEMIES), rc);

	
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::MORADO)		return POINTS;
	else if (type == ObjectType::ROJO)	return POINTS;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::MenosVidas() const
{
	if (type == ObjectType::MORADO)		return VIDA;
	else if (type == ObjectType::ROJO)	return VIDA;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}

