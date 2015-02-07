//
//  GameEndScene.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#include "GameEndScene.h"

USING_NS_CC;

#define LAYOUT_ORDER_BACKGROUND                 0
#define LAYOUT_ORDER_GAME_STATUS_LAYER          1

bool GameEndScene::init() {
    if (CCScene::init()) {
        // add background
        this->addBackground();
        // add status layer
        m_pGameEndStatusLayer = GameEndStatusLayer::create();
        this->addChild(m_pGameEndStatusLayer, LAYOUT_ORDER_GAME_STATUS_LAYER);
        
        return true;
    }
    return false;
}

GameEndScene::GameEndScene() {
    CCLOG("====================Game End Scene====================");
}

GameEndScene::~GameEndScene() {
    
}


void GameEndScene::onEnterTransitionDidFinish() {
    CCScene::onEnterTransitionDidFinish();
    
    // play effect
    if (GameStatusInstance.getPass()) {
        QIXAudioEngineInstance.playGamePass();
    } else {
        QIXAudioEngineInstance.playGameFail();
    }
}

void GameEndScene::onExit() {
    CCScene::onExit();
    
    // stop all sound effects
    QIXAudioEngineInstance.stopALLEffects();
}

void GameEndScene::addBackground() {
    const char* filename = QIXResourceInstance.m_oEndSceneConfig.BackgroundImage.c_str();
    CCRect rectEdge = QIXLayout::getBackgroundRect();
    m_pBackground = QIXCocos2dHelper::createSpriteWithRect(filename, rectEdge, kScaleExactFit);
    this->addChild(m_pBackground, LAYOUT_ORDER_BACKGROUND);
}