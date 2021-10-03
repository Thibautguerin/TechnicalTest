#include "Enemy.h"
#include "Game.h"

USING_NS_CC;

Enemy* Enemy::create(Vec2 position, Sprite* _sprite, float scale)
{
	Enemy* pRet = new(std::nothrow) Enemy();
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

bool Enemy::init(Vec2 position, Sprite* _sprite, float scale)
{
	sprite = _sprite;
	addChild(sprite);

	setPosition(position);
	setScale(scale);

	scheduleUpdate();
	return true;
}

void Enemy::update(float delta)
{
	if (isStarted)
	{
	}
}

void Enemy::Start()
{
	currentDelay = random(minShootingDelay, maxShootingDelay);
	isStarted = true;
}

void Enemy::Reset()
{
}

void Enemy::Stop()
{
	isStarted = false;
}

void Enemy::SetCamp(int id)
{
	campId = id;
}

void Enemy::SetShootingDifficulty(float _minShootingDelay, float _maxShootingDelay, float _randomIntervalPercentage)
{
	minShootingDelay = _minShootingDelay;
	maxShootingDelay = _maxShootingDelay;
	randomIntervalPercentage = _randomIntervalPercentage;
}

void Enemy::Shoot(Projectile& projectile)
{
	Game* game = static_cast<Game*>(getParent());
	game->AddProjectile(projectile);
	game->addChild(&projectile);
}

float Enemy::GetNewShootingDelay()
{
	Game* game = static_cast<Game*>(getParent());

	if (game != nullptr)
	{
		float newDelay = minShootingDelay + (maxShootingDelay - minShootingDelay) * (1 - game->GetDifficultyOverTime());
		float interval = newDelay * randomIntervalPercentage;
		return random(newDelay - interval, newDelay + interval);
	}
	else
		return 2.0f;
}
