#ifndef __TANK_H__
#define __TANK_H__

#pragma once

#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

class Tank : public Enemy
{
public:
	static Tank* create(Vec2 position, Sprite* _sprite, float scale = 1);
	virtual bool init(Vec2 position, Sprite* _sprite, float scale = 1);

	void update(float delta);
private:
};

#endif // __TANK_H__