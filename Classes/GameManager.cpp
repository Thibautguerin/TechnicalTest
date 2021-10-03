/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameManager.h"
#include "Tank.h"
#include "Plane.h"
#include "Projectile.h"

USING_NS_CC;

Scene* GameManager::createScene()
{
    return GameManager::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameManager::init()
{
    if (!Scene::init() || !Scene::initWithPhysics())
    {
        return false;
    }

    //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    EventListenerKeyboard *listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameManager::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameManager::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameManager::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    initScene(visibleSize, origin);
    initMainMenu(visibleSize, origin);
    initPauseMenu(visibleSize, origin);

    return true;
}

void GameManager::initScene(cocos2d::Size visibleSize, Vec2 origin)
{
    Sprite *backGround = Sprite::create("Background.png");
    if (backGround == nullptr)
    {
        problemLoading("'Background.png'");
    }
    else
    {
        backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        addChild(backGround, -1);
    }

    // Init map limits
    PhysicsBody* mapPhysicsBody = PhysicsBody::createBox(Size(visibleSize.width, 200), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0, -100));
    mapPhysicsBody->addShape(PhysicsShapeBox::create(Size(visibleSize.width, 200), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0, -visibleSize.height / 2 - 10)));
    mapPhysicsBody->addShape(PhysicsShapeBox::create(Size(200, 600), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(-visibleSize.width / 2 + 350, -250)));
    mapPhysicsBody->addShape(PhysicsShapeBox::create(Size(200, 600), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(visibleSize.width / 2 - 350, -250)));
    mapPhysicsBody->setDynamic(false);
    mapPhysicsBody->setCategoryBitmask(1 << 1);
    mapPhysicsBody->setCollisionBitmask(1 << 0);
    mapPhysicsBody->setContactTestBitmask(0);
    backGround->setTag(1);
    backGround->setPhysicsBody(mapPhysicsBody);

    Node* ProjectilesLimit = Node::create();
    ProjectilesLimit->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    PhysicsBody* limitPhysicsBody = PhysicsBody::createBox(Size(visibleSize.width, 200), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0, -visibleSize.height / 2 - 100));
    limitPhysicsBody->addShape(PhysicsShapeBox::create(Size(200, 600), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(-visibleSize.width / 2 + 150, -250)));
    limitPhysicsBody->addShape(PhysicsShapeBox::create(Size(200, 600), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(visibleSize.width / 2 - 150, -250)));
    limitPhysicsBody->setDynamic(false);
    limitPhysicsBody->setCategoryBitmask(1 << 2);
    limitPhysicsBody->setCollisionBitmask(1 << 3);
    limitPhysicsBody->setContactTestBitmask(1 << 3);
    ProjectilesLimit->setTag(2);
    ProjectilesLimit->setPhysicsBody(limitPhysicsBody);
    addChild(ProjectilesLimit);

    // Init Game

    // Init Player
    Sprite* playerSprite = Sprite::create("Player.png", Rect(0, 0, 80, 112));
    if (playerSprite == nullptr)
    {
        problemLoading("'Player.png'");
    }

    PhysicsBody *playerPhysicsBody = PhysicsBody::createCircle(25, PhysicsMaterial(0.1f, 1.0f, 0.0f), Vec2(0, -31));
    playerPhysicsBody->setDynamic(true);
    playerPhysicsBody->setGravityEnable(false);
    playerPhysicsBody->setRotationEnable(false);

    Player* player = Player::create(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 300), 3, 500, playerSprite, playerPhysicsBody);

    // Init Enemies
    Vector<Enemy*> enemies;
    enemies.reserve(10);

    for (int i = 0; i < 5; i++)
    {
        Sprite* tankSprite = Sprite::create("RedTank.png");
        if (tankSprite == nullptr)
        {
            problemLoading("'RedTank.png'");
        }
        float offset = i % 2 == 0 ? 250 : 280;
        enemies.pushBack(Tank::create(Vec2(offset ,origin.y + visibleSize.height / 2 - 220 - 70 * i), tankSprite, 1.5f));
    }

    for (int i = 0; i < 5; i++)
    {
        Sprite* tankSprite = Sprite::create("BlueTank.png");
        if (tankSprite == nullptr)
        {
            problemLoading("'BlueTank.png'");
        }
        float offset = i % 2 == 0 ? 250 : 280;
        Tank* newTank = Tank::create(Vec2(origin.y + visibleSize.width - offset, origin.y + visibleSize.height / 2 - 180 - 70 * i), tankSprite, 1.5f);
        newTank->SetCamp(1);
        enemies.pushBack(newTank);
    }

    game = Game::create(player, enemies);
    addChild(game);
}

void GameManager::initMainMenu(cocos2d::Size visibleSize, Vec2 origin)
{
    Vector<MenuItem*> menuItems;

    // Create background menu
    Sprite *background = Sprite::create("MenuBackground.png");
    if (background == nullptr)
    {
        problemLoading("'MenuBackground.png'");
    }
    else
    {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        background->setOpacity(200);
        menuItems.pushBack(MenuItemSprite::create(background, background, NULL));
    }

    // Create high score label

    //UserDefault::getInstance()->deleteValueForKey("HIGHSCORE");
    int highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE");
    std::string highScoreLabelText = "High Score: " + std::to_string(highScore);

    highScoreLabel = Label::createWithTTF(highScoreLabelText, "fonts/Marker Felt.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    if (highScoreLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        highScoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
        menuItems.pushBack(MenuItemLabel::create(highScoreLabel));
    }

    // Create last score label
    scoreLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 50, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    if (scoreLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
        menuItems.pushBack(MenuItemLabel::create(scoreLabel));
    }

    // Create start button
    MenuItemImage *startButton = MenuItemImage::create("Start.png", "Start.png", CC_CALLBACK_1(GameManager::startGameCallBack, this));
    if (startButton == nullptr
        || startButton->getContentSize().width <= 0
        || startButton->getContentSize().height <= 0)
    {
        problemLoading("'Start.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2;
        startButton->setPosition(Vec2(x, y));
        menuItems.pushBack(startButton);
    }

    // Create close game button
    MenuItemImage *closeButton = MenuItemImage::create("Quit.png", "Quit.png", CC_CALLBACK_1(GameManager::menuCloseCallBack, this));
    if (closeButton == nullptr
        || closeButton->getContentSize().width <= 0
        || closeButton->getContentSize().height <= 0)
    {
        problemLoading("'Quit.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 - closeButton->getContentSize().height - 30;
        closeButton->setPosition(Vec2(x, y));
        menuItems.pushBack(closeButton);
    }

    mainMenu = Menu::createWithArray(menuItems);
    mainMenu->setName("MainMenu");
    mainMenu->setPosition(Vec2::ZERO);

    addChild(mainMenu, 10);
}

void GameManager::initPauseMenu(cocos2d::Size visibleSize, Vec2 origin)
{
    Vector<MenuItem*> menuItems;

    // Create background menu
    Sprite *background = Sprite::create("MenuBackground.png");
    if (background == nullptr)
    {
        problemLoading("'MenuBackground.png'");
    }
    else
    {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        background->setOpacity(200);
        menuItems.pushBack(MenuItemSprite::create(background, background, NULL));
    }

    // Create Pause label
    Label *pause = Label::createWithTTF("PAUSE", "fonts/Marker Felt.ttf", 70);
    if (pause == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        pause->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
        menuItems.pushBack(MenuItemLabel::create(pause));
    }

    // Create resume button
    MenuItemImage *resumeButton = MenuItemImage::create("Resume.png", "Resume.png", CC_CALLBACK_1(GameManager::resumeCallBack, this));
    if (resumeButton == nullptr
        || resumeButton->getContentSize().width <= 0
        || resumeButton->getContentSize().height <= 0)
    {
        problemLoading("'Resume.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2;
        resumeButton->setPosition(Vec2(x, y));
        menuItems.pushBack(resumeButton);
    }

    // Create close game button
    MenuItemImage *closeButton = MenuItemImage::create("Quit.png", "Quit.png", CC_CALLBACK_1(GameManager::menuCloseCallBack, this));
    if (closeButton == nullptr
        || closeButton->getContentSize().width <= 0
        || closeButton->getContentSize().height <= 0)
    {
        problemLoading("'Quit.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 - closeButton->getContentSize().height - 30;
        closeButton->setPosition(Vec2(x, y));
        menuItems.pushBack(closeButton);
    }

    pauseMenu = Menu::createWithArray(menuItems);
    pauseMenu->setName("PauseMenu");
    pauseMenu->setPosition(Vec2::ZERO);

    hidePauseMenu();
    addChild(pauseMenu, 10);

}

void GameManager::Pause()
{
    getPhysicsWorld()->setSpeed(0);
    Director::getInstance()->pause();
    displayPauseMenu();
}

void GameManager::Resume()
{
    hidePauseMenu();
    Director::getInstance()->resume();
    getPhysicsWorld()->setSpeed(1);
}

void GameManager::displayMainMenu()
{
    if (mainMenu != nullptr)
    {
        mainMenu->setVisible(true);
        highScoreLabel->setString("High Score: " + std::to_string(game->GetHighScore()));
        scoreLabel->setString("Score: " + std::to_string(game->GetScore()));
    }
}

void GameManager::hideMainMenu()
{
    if (mainMenu != nullptr)
        mainMenu->setVisible(false);
}

void GameManager::displayPauseMenu()
{
    if (pauseMenu != nullptr)
        pauseMenu->setVisible(true);
}

void GameManager::hidePauseMenu()
{
    if (pauseMenu != nullptr)
        pauseMenu->setVisible(false);
}

void GameManager::StartGame()
{
    hideMainMenu();
    game->ResetGame();
    game->StartGame();
}

void GameManager::startGameCallBack(Ref* pSender)
{
    StartGame();
}

void GameManager::resumeCallBack(cocos2d::Ref* pSender)
{
    Resume();
}

void GameManager::menuCloseCallBack(Ref* pSender)
{
    Director::getInstance()->end();
}

void GameManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        if (mainMenu->isVisible())
        {
            Director::getInstance()->end();
        }
        else if (Director::getInstance()->isPaused())
        {
            Resume();
        }
        else
        {
            Pause();
        }
        break;
    case EventKeyboard::KeyCode::KEY_ENTER:
        if (mainMenu->isVisible())
        {
            StartGame();
        }
        else if (Director::getInstance()->isPaused())
        {
            Resume();
        }
        break;
    default:
        break;
    }
}

void GameManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

bool GameManager::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        if (nodeA->getTag() == 0) // Player damage
        {
            static_cast<Player*>(nodeA)->TakeDamage();
        }
        else if (nodeA->getTag() == 3) // projectile explosion
        {
            nodeA->removeFromParentAndCleanup(true);
        }

        if (nodeB->getTag() == 0) // Player damage
        {
            static_cast<Player*>(nodeB)->TakeDamage();
        }
        else if (nodeB->getTag() == 3) // projectile explosion
        {
            nodeB->removeFromParentAndCleanup(true);
        }
    }

    return true;
}
