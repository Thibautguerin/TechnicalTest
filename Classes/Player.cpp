#include "Player.h"
#include "Game.h"

USING_NS_CC;

Player *Player::create(Vec2 position, int _maxLives, float _speed, Sprite *_sprite, PhysicsBody *physicsBody)
{
	Player* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(position, _maxLives, _speed, _sprite, physicsBody))
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

bool Player::init(Vec2 position, int _maxLives, float _speed, Sprite *_sprite, PhysicsBody *physicsBody)
{
	maxLives = _maxLives;
	currentLives = maxLives;

	speed = _speed;

	sprite = _sprite;
	addChild(sprite);
    
    setTag(0);
    physicsBody->setCategoryBitmask(1 << 0);
    physicsBody->setCollisionBitmask(1 << 1 | 1 << 3);
    physicsBody->setContactTestBitmask(1 << 3);
    setPhysicsBody(physicsBody);

    defaultPosition = position;
	setPosition(position);

	EventListenerKeyboard* listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

	return true;
}

void Player::update(float delta)
{
    Move();
}

void Player::Move()
{
    Vec2 newVelocity = upVelocity + downVelocity + leftVelocity + rightVelocity;
    newVelocity.normalize();
	getPhysicsBody()->setVelocity(newVelocity * speed);
}

void Player::TakeDamage()
{
    currentLives = std::max(currentLives - 1, 0);
    Game* game = static_cast<Game*>(getParent());

    if (game != nullptr)
        game->UpdateLivesLabel(currentLives);
}

bool Player::isAlive() const
{
    return currentLives > 0;
}

int Player::GetLives() const
{
    return currentLives;
}

void Player::Reset()
{
    setPosition(defaultPosition);
    currentLives = maxLives;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    Game* game = static_cast<Game*>(getParent());
    if (game != nullptr && game->GetIsStarted() && !Director::getInstance()->isPaused())
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_Z:
            upVelocity = Vec2(0, 1);
            break;
        case EventKeyboard::KeyCode::KEY_W:
            upVelocity = Vec2(0, 1);
            break;
        case EventKeyboard::KeyCode::KEY_S:
            downVelocity = Vec2(0, -1);
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            leftVelocity = Vec2(-1, 0);
            break;
        case EventKeyboard::KeyCode::KEY_A:
            leftVelocity = Vec2(-1, 0);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            rightVelocity = Vec2(1, 0);
            break;
        default:
            break;
        }
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    Game* game = static_cast<Game*>(getParent());
    if (game != nullptr && game->GetIsStarted() && !Director::getInstance()->isPaused())
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_Z:
            upVelocity = Vec2::ZERO;
            break;
        case EventKeyboard::KeyCode::KEY_W:
            upVelocity = Vec2::ZERO;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            downVelocity = Vec2::ZERO;
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            leftVelocity = Vec2::ZERO;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            leftVelocity = Vec2::ZERO;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            rightVelocity = Vec2::ZERO;
            break;
        default:
            break;
        }
    }
}