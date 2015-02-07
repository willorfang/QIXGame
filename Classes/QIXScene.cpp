//
//  QIXScene.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/23/14.
//
//

#include "QIXScene.h"
#include "GameLoadScene.h"
#include "GameLoginScene.h"
#include "GameScene.h"
#include "GameEndScene.h"

USING_NS_CC;

void QIXScene::runGameLoadScene() {
    GameLoadScene *gameScene = GameLoadScene::create();
    CCDirector::sharedDirector()->runWithScene(gameScene);
}

void QIXScene::goToGameLoginScene() {
    QIXScene::goToNextScene<GameLoginScene>();
}

void QIXScene::goToGameScene() {
    QIXScene::goToNextScene<GameScene>();
}

void QIXScene::goToGameEndScene() {
    QIXScene::goToNextScene<GameEndScene>();
}

template<typename GAMESCENE>
void QIXScene::goToNextScene() {
    GAMESCENE *scene = GAMESCENE::create();
    CCTransitionScene* transition = CCTransitionSlideInR::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
}