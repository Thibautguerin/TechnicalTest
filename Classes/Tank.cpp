#include "Tank.h"

USING_NS_CC;

Tank* Tank::create(Vec2 position, Sprite* _sprite, float scale)
{
	Tank* pRet = new(std::nothrow) Tank();
	if (pRet && pRet->init(position, _sprite, scale))
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

bool Tank::init(Vec2 position, Sprite* _sprite, float scale)
{
	sprite = _sprite;
	addChild(sprite);

	setPosition(position);
	setScale(scale);

	scheduleUpdate();
	return true;
}

void Tank::update(float delta)
{
	if (isStarted)
	{
		if (currentDelay <= 0) // Shoot
		{
			Sprite* projectileSprite = Sprite::create("TankBullet.png");
			
			if (campId == 0)
			{
				PhysicsBody* physicsBody = PhysicsBody::createCircle(8, PHYSICSBODY_MATERIAL_DEFAULT, Vec2(20, 0));
				physicsBody->setDynamic(true);
				physicsBody->setGravityEnable(false);
				physicsBody->setRotationEnable(false);
				Shoot(*Projectile::create(getPosition() + Vec2(50, 30), Vec2(1, 0), 600, projectileSprite, physicsBody, 1.5));
			}
			else
			{
				PhysicsBody* physicsBody = PhysicsBody::createCircle(8, PHYSICSBODY_MATERIAL_DEFAULT, Vec2(-20, 0));
				physicsBody->setDynamic(true);
				physicsBody->setGravityEnable(false);
				physicsBody->setRotationEnable(false);
				Shoot(*Projectile::create(getPosition() + Vec2(-50, 30), Vec2(-1, 0), 600, projectileSprite, physicsBody, -1.5));
			}
			currentDelay = GetNewShootingDelay();
		}
		else
		{
			currentDelay -= delta;
		}
	}
}