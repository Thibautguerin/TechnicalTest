#ifndef __GAME_H__
#define __GAME_H__

#pragma once

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"

USING_NS_CC;

class Game : public Node
{
public:
	static Game* create(Player* _player, Vector<Enemy*> _enemies);
	virtual bool init(Player* _player, Vector<Enemy*> _enemie);

	void update(float delta);

	void StartGame();
	void ResetGame();
	void StopGame();

	void UpdateLivesLabel(int lives);
	void AddProjectile(Projectile& projectile);
	void setMaxDifficultyTime(float time);

	bool GetIsStarted() const;
	float GetDifficultyOverTime() const;
	int GetHighScore() const;
	int GetScore() const;

private:
	Player* player;
	Vector<Enemy*> enemies;
	Vector<Projectile*> projectiles;

	bool isStarted = false;
	int highScore;
	float score;

	float gameDuration;
	float maxDifficultyTime = 300;

	Label* highScoreLabel;
	Label* scoreLabel;
	Label* livesLabel;
};

#endif // __GAME_H__