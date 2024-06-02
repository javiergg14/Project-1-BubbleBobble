#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;
	particles = nullptr;

	Rectangle rc;
	const int n = 16;
	switch (type)
	{
	case ObjectType::EGG: rc = { n, 0, n, n }; break;
	case ObjectType::CARROT: rc = { 0, 0, n, n }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_FRUITS), rc);
	
}
Object::~Object()
{
}
void Object::SetParticleManager(ParticleManager* particles)
{
	this->particles = particles;
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	AABB box;
	if (type == ObjectType::EGG)
	{
		return POINTS_EGG;
		Point p;
		p.x = box.pos.x - (TILE_SIZE ) / 2;
		p.y = box.pos.y - (TILE_SIZE ) / 2;
		particles->Add(p);
	}
	else if (type == ObjectType::CARROT)
	{
		return POINTS_CARROT;
		Point p;
		p.x = box.pos.x - (TILE_SIZE) / 2;
		p.y = box.pos.y - (TILE_SIZE) / 2;
		particles->Add(p);
	}
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}

