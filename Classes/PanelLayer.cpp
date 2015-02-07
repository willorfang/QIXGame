//
//  PanelLayer.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/7/14.
//
//

#include "PanelLayer.h"
#include "QIXHelper.h"
#include "GameScene.h"

//! panel < panel item < panel label
#define LAYOUT_ORDER_PANEL          1
#define LAYOUT_ORDER_PANEL_ITEM     2
#define LAYOUT_ORDER_PANEL_LABEL    3
#define LAYOUT_ORDER_PANEL_TOP      4

USING_NS_CC;

bool PanelLayer::init() {
    if (CCLayer::init()) {
        // create top panel
        const char* topPanelImage = QIXResourceInstance.m_oGlobalConfig.TopPanelImage.c_str();
        const char* bottomPanelImage = QIXResourceInstance.m_oGlobalConfig.BottomPanelImage.c_str();
        const char* topPanelItemImage = QIXResourceInstance.m_oGlobalConfig.TopPanelItemImage.c_str();
        m_pTopPanel = QIXCocos2dHelper::createSpriteWithRect(topPanelImage, QIXLayout::getTopPanelRect(), kScaleExactFit);
        this->addChild(m_pTopPanel, LAYOUT_ORDER_PANEL);
        
        // create top panel labels
        std::vector<cocos2d::CCLabelBMFont*> labels;
        
        int enemyInitCount = Player::GetThePlayer()->getMapLayer()->getTotalEnemyCount();
        const char* strEnemy = CCString::createWithFormat("%d", enemyInitCount)->getCString();
        m_pEnemyCount = QIXCocos2dHelper::createWordArt(strEnemy);
        labels.push_back(m_pEnemyCount);
        
        int LaunchCountMax = QIXResourceInstance.m_oLevelConfig.PassCondition.LaunchCountMax;
        const char* strLaunch = CCString::createWithFormat("%d", LaunchCountMax)->getCString();
        m_pLaunchCount = QIXCocos2dHelper::createWordArt(strLaunch);
        labels.push_back(m_pLaunchCount);
        
        m_pAreaRatio = QIXCocos2dHelper::createWordArt("0%");
        labels.push_back(m_pAreaRatio);
        
        int PlayTimeMax = QIXResourceInstance.m_oLevelConfig.PassCondition.PlayTimeMax;
        const char* strPlayTime = CCString::createWithFormat("%d", PlayTimeMax)->getCString();
        m_pPlayTime = QIXCocos2dHelper::createWordArt(strPlayTime);
        labels.push_back(m_pPlayTime);
        
        // create panel item and add into scene graph tree
        std::vector<cocos2d::CCRect> itemRects = this->getTopPanelLayout(4);
        for (int i=0; i<4; ++i) {
            // create panel item
            CCSprite* panelItem = QIXCocos2dHelper::createSpriteWithRect(topPanelItemImage, itemRects[i], kScaleShowAll);
            this->addChild(panelItem, LAYOUT_ORDER_PANEL_ITEM);
            // set content rect of panel label
            CCRect labelRect = QIXCocos2dHelper::getRectByScale(itemRects[i], 0.8f);
            QIXCocos2dHelper::setContentRect(labels[i], labelRect, kScaleShowAll);
            // get default scale
            m_fDefaultScale = labels[i]->getScale();
            this->addChild(labels[i], LAYOUT_ORDER_PANEL_LABEL);
        }
        
        // initialize remaing life
        this->initializeRemaingLife();
        
        // bottom panel
        m_pBottomPanel = QIXCocos2dHelper::createSpriteWithRect(bottomPanelImage, QIXLayout::getBottomPanelRect(), kScaleExactFit);
        this->addChild(m_pBottomPanel, LAYOUT_ORDER_PANEL);
        
        return true;
    }
    
    return false;
}


PanelLayer::PanelLayer() {
    
}

PanelLayer::~PanelLayer() {
    
}

std::vector<cocos2d::CCRect> PanelLayer::getTopPanelLayout(int itemCount) {
    std::vector<cocos2d::CCRect> rectVec;
    CCRect panelRect = QIXLayout::getTopPanelRect();
    float padding = 30.0f;
    float itemWidth = (panelRect.size.width - padding * 2) / itemCount;
    float itemHeight = panelRect.size.height;
    for (int i=0; i<itemCount; ++i) {
        CCRect rect = CCRectMake(panelRect.origin.x + padding + i*itemWidth,
                                 panelRect.origin.y,
                                 itemWidth*0.9f,
                                 itemHeight*0.9f);
        rectVec.push_back(rect);
    }
    return rectVec;
}

std::vector<cocos2d::CCRect> PanelLayer::getBottomPanelLayout(int itemCount) {
    return this->getTopPanelLayout(itemCount);
}

void PanelLayer::setEnemyCount(int count) {
    const char* strEnemy = CCString::createWithFormat("%d", count)->getCString();
    m_pEnemyCount->setString(strEnemy);
    // run label effect
    this->runDefaultFontEffect(m_pEnemyCount);
}

void PanelLayer::setLaunchCount(int count) {
    const char* strLaunch = CCString::createWithFormat("%d", count)->getCString();
    m_pLaunchCount->setString(strLaunch);
    // run label effect
    if (0 == count) {
        CCBlink* blink = CCBlink::create(0.5f, 8);
        CCRepeatForever* repeat = CCRepeatForever::create(blink);
        m_pLaunchCount->runAction(repeat);
    } else {
        this->runDefaultFontEffect(m_pLaunchCount);
    }
}

void PanelLayer::setPlayTime(int count) {
    const char* strPlayTime = CCString::createWithFormat("%d", count)->getCString();
    m_pPlayTime->setString(strPlayTime);
    // run label effect
    if (0 == count) {
        CCBlink* blink = CCBlink::create(0.5f, 8);
        CCRepeatForever* repeat = CCRepeatForever::create(blink);
        m_pPlayTime->runAction(repeat);
    } else {
        this->runDefaultFontEffect(m_pPlayTime);
    }
}

void PanelLayer::setAreaRatio(int startValue, int endValue) {
    // stop the previous actions if any
    PanelLayer::stopDefaultFontEffect(m_pAreaRatio);
    
    // run the action
    CCCallFuncND* callbackFunc = CCCallFuncND::create(this, callfuncND_selector(PanelLayer::defaultFontCallFunc), NULL);
    CCActionInterval* action = QIXEffect::createScaleOutAndBackWithShow(startValue, endValue, callbackFunc);
    m_pAreaRatio->runAction(action);
}


void PanelLayer::resetDefaultFontEffect(cocos2d::CCLabelBMFont* label) {
    label->setScale(m_fDefaultScale);
}

void PanelLayer::stopDefaultFontEffect(cocos2d::CCLabelBMFont* label) {
    // stop all actions
    label->stopAllActions();
    // reset attributes
    PanelLayer::resetDefaultFontEffect(label);
}

void PanelLayer::runDefaultFontEffect(CCLabelBMFont* label) {
    // stop the previous actions if any
    PanelLayer::stopDefaultFontEffect(label);
    
    // run the action
    CCCallFuncND* callbackFunc = CCCallFuncND::create(this, callfuncND_selector(PanelLayer::defaultFontCallFunc), NULL);
    CCActionInterval* action = QIXEffect::createScaleOutAndBack(1, callbackFunc);
    label->runAction(action);
}

void PanelLayer::defaultFontCallFunc(cocos2d::CCNode *node, void* data) {
    node->setScale(m_fDefaultScale);
}

void PanelLayer::initializeRemaingLife() {
    int MissCountMax = QIXResourceInstance.m_oLevelConfig.PassCondition.MissCountMax;
    int lifeCount = MissCountMax;
    CCSize size = QIXResourceInstance.m_oGlobalConfig.PlayerSize;
    const char* fileSafe = QIXResourceInstance.m_oGlobalConfig.PlayerSafeImage.c_str();
    CCPoint origin = QIXLayout::getScreenRect().origin;
    
    for (int i=0; i<lifeCount; ++i) {
        CCRect rect = CCRectMake(origin.x + i*size.width,
                                 origin.y,
                                 size.width,
                                 size.height);
        // create new life
        CCSprite* sprite = QIXCocos2dHelper::createSpriteWithRect(fileSafe, rect, kScaleExactFit);
        // add animation
//        float delay = CCRANDOM_0_1() * 2.0f;
//        this->runDelayedBounceForeverEffect(sprite, delay);
        // add into scene graph
        m_sRemainingLife.push(sprite);
        this->addChild(sprite, LAYOUT_ORDER_PANEL_LABEL);
    }
}

void PanelLayer::decreaseOneLife() {
    CCSprite* sprite = m_sRemainingLife.top();
    CCAssert(NULL != sprite, "");
    m_sRemainingLife.pop();
    
    if (0 == m_sRemainingLife.size()) {
        CCBlink* blink = CCBlink::create(0.5f, 8);
        CCRepeatForever* repeat = CCRepeatForever::create(blink);
        sprite->runAction(repeat);
    } else {
        this->removeChild(sprite);
    }
}

void PanelLayer::runDelayedBounceForeverEffect(cocos2d::CCSprite *sprite, float delay) {
    CCDelayTime* delayTime = CCDelayTime::create(delay);
    CCCallFuncN* callbackFunc = CCCallFuncN::create(this, callfuncN_selector(PanelLayer::delayedBounceForeverCallFunc));
    CCSequence* seq = CCSequence::create(delayTime, callbackFunc, NULL);
    sprite->runAction(seq);
}

void PanelLayer::delayedBounceForeverCallFunc(cocos2d::CCNode* node) {
    CCActionInterval* action = QIXEffect::createBounceUpAndBack();
    node->runAction(action);
}