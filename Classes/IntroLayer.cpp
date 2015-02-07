//
//  IntroLayer.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/27/14.
//
//

#include "IntroLayer.h"
#include "QIXHelper.h"
#include "GameScene.h"

#define LAYOUT_ORDER_PANEL          1
#define LAYOUT_ORDER_PANEL_LABEL    2

USING_NS_CC;

bool IntroLayer::init() {
    if (CCLayer::init()) {
        // initialize intro panel
        this->initializeIntroPanel();
        return true;
    }
    return false;
}

IntroLayer::IntroLayer() {
    
}

IntroLayer::~IntroLayer() {
    
}

void IntroLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool IntroLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void IntroLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void IntroLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint position = this->locationFromTouch(touch);
    if (m_pIntroPanel->boundingBox().containsPoint(position)) {
        m_pIntroPanel->setVisible(false);
        this->setTouchEnabled(false);
        
        // send message
        CCLOG("Send message {%s}", CCNC_MSG_INTRO_PANEL_TOUCHED);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(CCNC_MSG_INTRO_PANEL_TOUCHED);
    }
}

CCPoint IntroLayer::locationFromTouch(CCTouch* touch)
{
    return CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
}

void IntroLayer::initializeIntroPanel() {
    // background
    const char* filename = QIXResourceInstance.m_oGlobalConfig.IntroPanelBackgroundImage.c_str();
    CCRect rect = QIXLayout::getScreenRect();
    m_pIntroPanel = QIXCocos2dHelper::createSpriteWithRect(filename, rect, kScaleShowAll);
    CCAssert(NULL != m_pIntroPanel, "");
    this->addChild(m_pIntroPanel, LAYOUT_ORDER_PANEL);
}

void IntroLayer::showIntro(string title, string info) {
    // enable visiable
    m_pIntroPanel->setVisible(true);
    // clear all labels
    m_pIntroPanel->removeAllChildren();
    
    float introPanelWidth = m_pIntroPanel->boundingBox().size.width;
    float introPanelHeight = m_pIntroPanel->boundingBox().size.height;
    // add title
    CCPoint levelPos = ccp(introPanelWidth*0.5, introPanelHeight*0.7);
    CCLabelBMFont* level = QIXCocos2dHelper::createWordArt(title.c_str());
    level->setPosition(levelPos);
    m_pIntroPanel->addChild(level);
    
    // add intro content
    const char* fontName = "Helvetica";
    float fontSize = 30.0f;
    CCPoint introPos = ccp(introPanelWidth*0.5, introPanelHeight*0.4);
    CCLabelTTF* content = CCLabelTTF::create(info.c_str(), fontName, fontSize);
    content->setPosition(introPos);
    content->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pIntroPanel->addChild(content, LAYOUT_ORDER_PANEL_LABEL);
    
    // add animation
    CCCallFuncND* callbackFunc = CCCallFuncND::create(this, callfuncND_selector(IntroLayer::introPanelCallFunc), NULL);
    CCActionInterval* action = QIXEffect::createMoveInOutWithEase(m_pIntroPanel,
                                                                  kDirectionFromTop,
                                                                  callbackFunc);
    m_pIntroPanel->runAction(action);
}

void IntroLayer::showTargetIntro() {
    // title
    string title = CCString::createWithFormat("LEVEL %d", GameStatusInstance.getLevel())->getCString();
    // conent
    string content;
    int PlayTimeMax = QIXResourceInstance.m_oLevelConfig.PassCondition.PlayTimeMax;
    int LaunchCountMax = QIXResourceInstance.m_oLevelConfig.PassCondition.LaunchCountMax;
    int AreaClearRatio = QIXResourceInstance.m_oLevelConfig.PassCondition.AreaClearRatio;
    
    //
    content += "-- Kill all enemies\n";
    //
    content += CCString::createWithFormat("-- Player launch count <= %d\n", LaunchCountMax)->getCString();
    //
    content += CCString::createWithFormat("-- Map area clear ratio >= %d%%\n", AreaClearRatio)->getCString();
    //
    content += CCString::createWithFormat("-- Play time <= %ds\n", PlayTimeMax)->getCString();
    
    // add as intro
    this->showIntro(title, content);
}

void IntroLayer::showResultIntroAfterSomeTime(float time) {
    this->scheduleOnce(schedule_selector(IntroLayer::showResultIntro), time);
}

void IntroLayer::showResultIntro() {
    string title;
    string content;
    
    if (GameStatusInstance.getPass()) {
        // title
        title = CCString::createWithFormat("LEVEL %d PASS", GameStatusInstance.getLevel())->getCString();
        // content
        content = "Well Done!";
    } else {
        // title
        title = CCString::createWithFormat("LEVEL %d FAIL", GameStatusInstance.getLevel())->getCString();
        // conent
        int remainingEnemyCount = GameStatusInstance.getTotalEnemyCount() - GameStatusInstance.getKilledEnemyCount();
        int remainingMissCount = QIXResourceInstance.m_oLevelConfig.PassCondition.MissCountMax - GameStatusInstance.getPlayerMissCount();
        int remainingLaunchCount = QIXResourceInstance.m_oLevelConfig.PassCondition.LaunchCountMax - GameStatusInstance.getLaunchCount();
        int areaClearRatio = GameStatusInstance.getAreaClearRatio();
        int areaClearRatioMax = QIXResourceInstance.m_oLevelConfig.PassCondition.AreaClearRatio;
        int remainingPlayTime = QIXResourceInstance.m_oLevelConfig.PassCondition.PlayTimeMax - GameStatusInstance.getPlayTime();
        //
        if (remainingEnemyCount > 0) {
            content += CCString::createWithFormat("-- Remaining enemies: %d\n", remainingEnemyCount)->getCString();
        }
        //
        if (remainingMissCount == 0) {
            content += "-- No remaining life\n";
        }
        //
        if (remainingLaunchCount == 0) {
            content += "-- No remaining launch\n";
        }
        //
        if (remainingPlayTime == 0) {
            content += "-- No remaining time\n";
        }
        //
        if (areaClearRatio < areaClearRatioMax) {
            content +=  CCString::createWithFormat("-- Not reach AreaClearRatio: %d%%\n", areaClearRatioMax)->getCString();
        }
    }
    
    // add as intro
    this->showIntro(title, content);
}

void IntroLayer::introPanelCallFunc(cocos2d::CCNode* node, void* data) {
    this->setTouchEnabled(true);
}