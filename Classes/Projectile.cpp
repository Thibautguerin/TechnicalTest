#include "Projectile.h"

USING_NS_CC;

Projectile* Projectile::create(Vec2 position, Vec2 _direction, float _speed, Sprite *_sprite, PhysicsBody *physicsBody, float scale)
{
	Projectile* pRet = new(std::nothrow) Projectile();
	if (pRet && pRet->init(position, _direction, _speed, _sprite, physicsBody, scale))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Projectile::init(Vec2 position, Vec2 _direction, float _speed, Sprite *_sprite, PhysicsBody *physicsBody, float scale)
{
	direction = _direction;
	speed = _speed;

	sprite = _sprite;
	addChild(sprite);

	setPosition(position);
	setScale(scale);

	setTag(3);
	physicsBody->setCategoryBitmask(1 << 3);
	physicsBody->setCollisionBitmask(1 << 0 | 1 << 2 | 1 << 3);
	physicsBody->setContactTestBitmask(1 << 0 | 1 << 2 | 1 << 3);
	physicsBody->setVelocity(direction * speed);
	setPhysicsBody(physicsBody);

	return true;
}