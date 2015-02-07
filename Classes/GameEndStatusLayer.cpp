//
//  GameEndStatusLayer.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#include "GameEndStatusLayer.h"
#include "GameScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define LAYOUT_ORDER_LABEL_BACKGROUND       0
#define LAYOUT_ORDER_LABEL                  1

bool GameEndStatusLayer::init() {
    if (CCLayer::init()) {
        this->addGameStatusLabel();
        this->addScoreLabel();
        this->addScoreData();
        this->addReplayButton();
        this->addNextButton();
        return true;
    }
    return false;
}

GameEndStatusLayer::GameEndStatusLayer() {

}

GameEndStatusLayer::~GameEndStatusLayer() {
    
}

void GameEndStatusLayer::addGameStatusLabel() {
    const char* name = CCString::createWithFormat("Stage %s", GameStatusInstance.getPass() ? "Pass" : "Fail")->getCString();
    const LabelConfig& config = QIXResourceInstance.m_oEndSceneConfig.GameStatusLabel;
    
    // config
    QIXCocos2dHelper::configLabel(m_pGameStatusLabel, name, config);
    
    // run animation
    CCActionInterval* action = QIXEffect::createMoveInOutWithEase(m_pGameStatusLabel, kDirectionFromTop);
    m_pGameStatusLabel->runAction(action);
    
    // add into scene graph
    this->addChild(m_pGameStatusLabel, LAYOUT_ORDER_LABEL);
}

void GameEndStatusLayer::addScoreLabel() {
    const char* name = "Score";
    const LabelConfig& config = QIXResourceInstance.m_oEndSceneConfig.ScoreLabel;
    
    // config
    QIXCocos2dHelper::configLabel(m_pScoreLabel, name, config);
    
    // run animation
    CCActionInterval* action = QIXEffect::createMoveInOutWithEase(m_pScoreLabel, kDirectionFromLeft);
    m_pScoreLabel->runAction(action);
    
    // add into scene graph
    this->addChild(m_pScoreLabel, LAYOUT_ORDER_LABEL);
}

void GameEndStatusLayer::addScoreData() {
    // get config
    const ButtonConfig& config = QIXResourceInstance.m_oEndSceneConfig.ScoreData;
    const char* fontName = config.Font.c_str();
    const char* backgroundName = config.Image.c_str();
    CCPoint pos = config.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    CCSize size = config.Size;
    
    // add background
    CCRect rectBackground = CCRectMake(pos.x - size.width*0.5,
                                       pos.y - size.height*0.5,
                                       size.width,
                                       size.height);
    m_pScoreDataNode = QIXCocos2dHelper::createSpriteWithRect(backgroundName, rectBackground, kScaleShowAll);
    
    // run animation
    CCActionInterval* action = QIXEffect::createMoveInOutWithEase(m_pScoreDataNode, kDirectionFromRight);
    m_pScoreDataNode->runAction(action);
    
    // add into scene graph
    this->addChild(m_pScoreDataNode, LAYOUT_ORDER_LABEL);
    
    // add score data
    CCSize contentSize = m_pScoreDataNode->getContentSize();
    CCSize scoreDataSize = CCSizeMake(contentSize.width*0.5, contentSize.height*0.5);
    CCRect rectScore = CCRectMake((contentSize.width-scoreDataSize.width)*0.5,
                                  (contentSize.height-scoreDataSize.height)*0.5,
                                  scoreDataSize.width,
                                  scoreDataSize.height);
    // TODO, update score data HERE
    m_pScoreData = QIXCocos2dHelper::createWordArt("100", fontName);
    QIXCocos2dHelper::setContentRect(m_pScoreData, rectScore, kScaleShowAll);
    m_pScoreDataNode->addChild(m_pScoreData);
}

void GameEndStatusLayer::addReplayButton() {
    const char * labelName = "Replay";
    const ButtonConfig& config = QIXResourceInstance.m_oEndSceneConfig.ReplayButton;
    
    // config
    QIXCocos2dHelper::configButton(m_pReplayButton, labelName, config);
    
    // add action
    m_pReplayButton->addTargetWithActionForControlEvents(this,
                                                   cccontrol_selector(GameEndStatusLayer::replayButtonCallBack),
                                                   CCControlEventTouchUpInside);
    
    // run animation
    CCActionInterval* actionCreep = QIXEffect::createCreep();
    CCActionInterval* actionMoveIn = QIXEffect::createMoveInOutWithEase(m_pReplayButton, kDirectionFromBottom);
    m_pReplayButton->runAction(actionCreep);
    m_pReplayButton->runAction(actionMoveIn);
    
    // add into scene graph
    this->addChild(m_pReplayButton, LAYOUT_ORDER_LABEL);
}

void GameEndStatusLayer::addNextButton() {
    const char * labelName = "Next";
    const ButtonConfig& config = QIXResourceInstance.m_oEndSceneConfig.NextButton;
    
    // config
    QIXCocos2dHelper::configButton(m_pNextButton, labelName, config);
    
    // add action
    m_pNextButton->addTargetWithActionForControlEvents(this,
                                                         cccontrol_selector(GameEndStatusLayer::nextButtonCallBack),
                                                         CCControlEventTouchUpInside);
    
    // run animation
    CCActionInterval* actionMoveIn = QIXEffect::createMoveInOutWithEase(m_pNextButton, kDirectionFromBottom);
    m_pNextButton->runAction(actionMoveIn);
    
    //set disabled
    if (GameStatusInstance.getPass()) {
        CCActionInterval* actionCreep = QIXEffect::createCreep();
        m_pNextButton->runAction(actionCreep);
    } else {
        m_pNextButton->setEnabled(false);
    }
    
    // add into scene graph
    this->addChild(m_pNextButton, LAYOUT_ORDER_LABEL);
}

void GameEndStatusLayer::replayButtonCallBack(cocos2d::CCObject *object, CCControlEvent controlEvent) {
    // reset game status
    GameStatusInstance.reset();
    // go to game scene
    QIXScene::goToGameScene();
}

void GameEndStatusLayer::nextButtonCallBack(cocos2d::CCObject *object,
                        cocos2d::extension::CCControlEvent controlEvent) {
    // go to next level
    GameStatusInstance.goToNextLevel();
    // reset game status
    GameStatusInstance.reset();
    // go to game scene
    QIXScene::goToGameScene();
}