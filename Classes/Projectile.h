#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Projectile : public Node
{
public:
	static Projectile *create(Vec2 position, Vec2 _direction, float _speed, Sprite *_sprite, PhysicsBody* physicsBody, float scale = 1);
	virtual bool init(Vec2 position, Vec2 _direction, float _speed, Sprite* _sprite, PhysicsBody* physicsBody, float scale = 1);

private:
	Vec2 direction;
	float speed;
	Sprite* sprite;
};

#endif // __PROJECTILE_H__