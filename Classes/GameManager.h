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

#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"
#include "Game.h"

USING_NS_CC;

class GameManager : public Scene
{
public:
    static Scene* createScene();

    virtual bool init();
    
    void initScene(Size visibleSize, Vec2 origin);
    void initMainMenu(Size visibleSize, Vec2 origin);
    void initPauseMenu(Size visibleSize, Vec2 origin);

    void Pause();
    void Resume();

    void displayMainMenu();
    void hideMainMenu();
    void displayPauseMenu();
    void hidePauseMenu();

    void StartGame();

    // a selector callback
    void startGameCallBack(Ref* pSender);
    void resumeCallBack(Ref* pSender);
    void menuCloseCallBack(Ref* pSender);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    bool onContactBegin(PhysicsContact& contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameManager);

private:

    Game* game;

    Menu* mainMenu;
    Menu* pauseMenu;

    Label* highScoreLabel;
    Label* scoreLabel;
};

#endif // __GAMEMANAGER_H__
