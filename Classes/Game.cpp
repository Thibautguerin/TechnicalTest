#include "Game.h"
#include "GameManager.h"

USING_NS_CC;

Game *Game::create(Player* _player, Vector<Enemy*> _enemies)
{
	Game* pRet = new(std::nothrow) Game();
	if (pRet && pRet->init(_player, _enemies))
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

bool Game::init(Player* _player, Vector<Enemy*> _enemies)
{
	player = _player;
	addChild(player);

	enemies = _enemies;
	for each (Enemy * enemy in enemies)
	{
		addChild(enemy);
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//UserDefault::getInstance()->deleteValueForKey("HIGHSCORE");
	highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE");
	std::string highScoreLabelText = "HS: " + std::to_string(highScore);

	// Create high score label
	highScoreLabel = Label::createWithTTF(highScoreLabelText, "fonts/Marker Felt.ttf", 50, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	if (highScoreLabel != nullptr)
	{
		highScoreLabel->setTextColor(Color4B(150, 150, 150, 255));
		highScoreLabel->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - highScoreLabel->getContentSize().height / 2 - 10));
		addChild(highScoreLabel);
	}

	// Create score label
	scoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	if (scoreLabel != nullptr)
	{
		scoreLabel->setTextColor(Color4B(130, 130, 130, 255));
		scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		addChild(scoreLabel);
	}

	// Create lives label
	livesLabel = Label::createWithTTF(std::to_string(player->GetLives()), "fonts/Marker Felt.ttf", 50, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	if (livesLabel != nullptr)
	{
		livesLabel->setTextColor(Color4B(150, 150, 150, 255));
		livesLabel->setPosition(Vec2(origin.x + visibleSize.width - 90, origin.y + visibleSize.height - livesLabel->getContentSize().height / 2 - 10));
		addChild(livesLabel);
	}
	Sprite* livesSprite = Sprite::create("Life.png");
	if (livesSprite != nullptr)
	{
		livesSprite->setAnchorPoint(Vec2(1, 1));
		livesSprite->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height - 10));
		addChild(livesSprite);
	}

	scheduleUpdate();
	return true;
}

void Game::update(float delta)
{
	if (isStarted)
	{
		if (player->isAlive())
		{
			gameDuration = std::min(gameDuration + delta, maxDifficultyTime);
			score += delta * 10;
			scoreLabel->setString(std::to_string(static_cast<int>(score)));
			if (static_cast<int>(score) > highScore)
			{
				UserDefault* userDefault = UserDefault::getInstance();
				userDefault->setIntegerForKey("HIGHSCORE", score);
				userDefault->flush();

				highScore = score;
				highScoreLabel->setString("HS: " + std::to_string(highScore));
			}
		}
		else
		{
			StopGame();
		}
	}
}

void Game::StartGame()
{
	isStarted = true;
	for each (Enemy *enemy in enemies)
	{
		enemy->Start();
	}
}

void Game::ResetGame()
{
	score = 0;
	gameDuration = 0;
	scoreLabel->setString("0");
	player->Reset();
	livesLabel->setString(std::to_string(player->GetLives()));
	for each (Enemy *enemy in enemies)
	{
		enemy->Reset();
	}
	for each (Projectile * projectile in projectiles)
	{
		projectile->removeFromParentAndCleanup(true);
	}
}

void Game::StopGame()
{
	isStarted = false;
	for each (Enemy *enemy in enemies)
	{
		enemy->Stop();
	}

	GameManager* gameManager = static_cast<GameManager*>(getParent());
	if (gameManager != nullptr)
	{
		gameManager->displayMainMenu();
	}
}

void Game::UpdateLivesLabel(int lives)
{
	livesLabel->setString(std::to_string(lives));
}

void Game::AddProjectile(Projectile& projectile)
{
	projectiles.pushBack(&projectile);
}

void Game::setMaxDifficultyTime(float time)
{
	if (time < gameDuration)
	{
		gameDuration = time;
	}
	maxDifficultyTime = time;
}

bool Game::GetIsStarted() const
{
	return isStarted;
}

float Game::GetDifficultyOverTime() const
{
	return gameDuration / maxDifficultyTime;
}

int Game::GetHighScore() const
{
	return highScore;
}

int Game::GetScore() const
{
	return static_cast<int>(score);
}
