#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Player : public Node
{
public:
	static Player *create(Vec2 position, int _maxLives, float _speed, Sprite *_sprite, PhysicsBody *physicsBody);
	virtual bool init(Vec2 position, int _maxLives, float _speed, Sprite *_sprite, PhysicsBody *physicsBody);

	void update(float delta);
	void Move();
	void TakeDamage();

	bool isAlive() const;
	int GetLives() const;

	void Reset();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
	int maxLives;
	int currentLives;
	float speed;
	Vec2 defaultPosition;

	Sprite* sprite;

	Vec2 upVelocity = Vec2::ZERO;
	Vec2 downVelocity = Vec2::ZERO;
	Vec2 leftVelocity = Vec2::ZERO;
	Vec2 rightVelocity = Vec2::ZERO;
};

#endif // __PLAYER_H__