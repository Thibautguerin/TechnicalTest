#ifndef __ENEMY_H__
#define __ENEMY_H__

#pragma once

#include "cocos2d.h"
#include "Projectile.h"

USING_NS_CC;

class Enemy : public Node
{
public:
	static Enemy* create(Vec2 position, Sprite* _sprite, float scale = 1);
	virtual bool init(Vec2 position, Sprite* _sprite, float scale = 1);

	void update(float delta);

	void Start();
	void Reset();
	void Stop();

	void SetCamp(int id);
	void SetShootingDifficulty(float _minShootingDelay, float _maxShootingDelay, float _randomIntervalPercentage);
	void Shoot(Projectile& projectile);

	float GetNewShootingDelay();

protected:
	Sprite* sprite;
	bool isStarted = false;
	int campId = 0;

	float currentDelay;

	float minShootingDelay = 2.0f;
	float maxShootingDelay = 7.0f;
	float randomIntervalPercentage = 0.2f;
};

#endif // __ENEMY_H__