//
//  GameLoginScene.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/19/14.
//
//

#include "GameLoginScene.h"
#include "GameScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define LAYOUT_ORDER_BACKGROUND             0
#define LAYOUT_ORDER_LABEL_BACKGROUND       1
#define LAYOUT_ORDER_LABEL                  2

bool GameLoginScene::init() {
    if (CCScene::init()) {
        // init scene
        this->addBackground();
        this->addTitleLabel();
        this->addPlayButton();
        return true;
    }
    return false;
}


void GameLoginScene::onEnterTransitionDidFinish() {
    CCScene::onEnterTransitionDidFinish();
    
    // play background music
    QIXAudioEngineInstance.playBackgroundMusic();
}

void GameLoginScene::onExit() {
    CCScene::onExit();
    
    // stop background music
    QIXAudioEngineInstance.stopBackgroundMusic();
}

GameLoginScene::GameLoginScene() {
    CCLOG("====================Game Login Scene====================");
}

GameLoginScene::~GameLoginScene() {
    
}

void GameLoginScene::addBackground() {
    const char* filename = QIXResourceInstance.m_oLoginSceneConfig.BackgroundImage.c_str();
    CCRect rectEdge = QIXLayout::getBackgroundRect();
    m_pBackground = QIXCocos2dHelper::createSpriteWithRect(filename, rectEdge, kScaleExactFit);
    this->addChild(m_pBackground, LAYOUT_ORDER_BACKGROUND);
}

void GameLoginScene::addTitleLabel() {
    const char* name = "QIXGame";
    const LabelConfig& config = QIXResourceInstance.m_oLoginSceneConfig.TitleLabel;
    // config
    QIXCocos2dHelper::configLabel(m_pGameTitleLabel, name, config);
    this->addChild(m_pGameTitleLabel, LAYOUT_ORDER_LABEL);
}

void GameLoginScene::addPlayButton() {
    const char * labelName = "Play";
    const ButtonConfig& config = QIXResourceInstance.m_oLoginSceneConfig.PlayButton;
    // config
    QIXCocos2dHelper::configButton(m_pButton, labelName, config);
    // add action
    m_pButton->addTargetWithActionForControlEvents(this,
                                                   cccontrol_selector(GameLoginScene::playButtonCallBack),
                                                   CCControlEventTouchUpInside);
    // add animation
    CCActionInterval* action = QIXEffect::createEmphasizeButton(m_pButton->getScale());
    m_pButton->runAction(action);
    // add into scene graph
    this->addChild(m_pButton, LAYOUT_ORDER_LABEL);
}

void GameLoginScene::playButtonCallBack(cocos2d::CCObject *object, CCControlEvent controlEvent) {
    // go to next scene
    QIXScene::goToGameScene();
}