//
//  PlayerLayer.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#include "PlayerLayer.h"
#include "MapArea.h"
#include "GameScene.h"

//! player < asist
#define LAYOUT_ORDER_PLAYER_TRACK   1
#define LAYOUT_ORDER_PLAYER         2
#define LAYOUT_ORDER_PLAYER_ASIST   3
#define LAYOUT_ORDER_PLAYER_EFFECT  4

USING_NS_CC;

bool PlayerLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    // default setting
    m_bIsLaunching = false;
    m_bIsLaunchCanceled = false;
    
    // create the player
    m_pPlayer = Player::GetThePlayer();
    CCAssert(NULL != m_pPlayer, "");
    this->addChild(m_pPlayer, LAYOUT_ORDER_PLAYER);
    
    // initialize player effect
    m_pPlayerEffect = PlayerEffect::create();
    CCAssert(NULL != m_pPlayerEffect, "");
    this->addChild(m_pPlayerEffect, LAYOUT_ORDER_PLAYER_EFFECT);
    
    // initialize player track
    m_pPlayerTrack = PlayerTrack::create();
    CCAssert(NULL != m_pPlayerTrack, "");
    this->addChild(m_pPlayerTrack, LAYOUT_ORDER_PLAYER_TRACK);
    
    // create arrow sprite
    this->addArrow();
    // create ban sprite
    this->addBan();
    
    return true;
}

PlayerLayer::PlayerLayer() {
    
}

PlayerLayer::~PlayerLayer() {
}

void PlayerLayer::resetPlayer() {
    // kill the player
    m_pPlayer->beKilled();
    // clear player's path points
    m_pPlayer->getPlayerPath()->clearPathPoints();
    // clear player's track
    m_pPlayerTrack->endTrack();
}

void PlayerLayer::attractPlayer(cocos2d::CCPoint point) {
    // attract the player
    m_pPlayer->beAttracted(point);
    // clear player's path points
    m_pPlayer->getPlayerPath()->clearPathPoints();
    // clear player's track
    m_pPlayerTrack->endTrack();
}

void PlayerLayer::addArrow() {
    const char *filename = QIXResourceInstance.m_oGlobalConfig.PlayerLaunchArrowImage.c_str();
    m_pArrowSprite = QIXCocos2dHelper::createSprite(filename);
    // set the opacity [0, 255]
    m_pArrowSprite->setVisible(false);
    m_pArrowSprite->setOpacity(125.0f);
    // set size and anchor
    float scale = QIXResourceInstance.m_oGlobalConfig.PlayerArrowLengthDefault / m_pArrowSprite->getContentSize().width;
    m_pArrowSprite->setScale(scale);
    m_pArrowSprite->setAnchorPoint(ccp(0.0f, 0.5f));
    this->addChild(m_pArrowSprite, LAYOUT_ORDER_PLAYER_ASIST);
}

void PlayerLayer::addBan() {
    const char *filename = QIXResourceInstance.m_oGlobalConfig.PlayerLaunchBanImage.c_str();
    m_pBanSprite = QIXCocos2dHelper::createSprite(filename);
    // set the opacity [0, 255]
    m_pBanSprite->setVisible(false);
    // set size and anchor
    CCSize size = QIXResourceInstance.m_oGlobalConfig.ItemSizeDefault;
    float scale = size.height / m_pBanSprite->getContentSize().height;
    m_pBanSprite->setScale(scale);
    this->addChild(m_pBanSprite, LAYOUT_ORDER_PLAYER_ASIST);
}

void PlayerLayer::showBan(cocos2d::CCPoint point) {
    m_pBanSprite->setPosition(point);
    m_pBanSprite->setVisible(true);
    // run action
    CCDelayTime* delayTime = CCDelayTime::create(1.0f);
    CCCallFunc* callbackFunc = CCCallFunc::create(this, callfunc_selector(PlayerLayer::showBanCallback));
    CCSequence* action = CCSequence::create(delayTime, callbackFunc, NULL);
    m_pBanSprite->runAction(action);
    // play ban sound
    QIXAudioEngineInstance.playTouchBan();
}

void PlayerLayer::showBanCallback() {
    m_pBanSprite->setVisible(false);
}

bool PlayerLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    // filter extra touched when it is "cancelled" state
    if (m_bIsLaunchCanceled) {
        return false;
    }
    
    // cancel the launch when it is "launching" state
    if (m_bIsLaunching) {
        m_bIsLaunchCanceled = true;
        m_pArrowSprite->setVisible(false);
        return false;
    }
    
    // get touch position
    CCPoint position = this->locationFromTouch(touch);
    m_vTouchPoints.first = position;
//    CCLOG("Touch begin position: [%f, %f]", position.x, position.y);

    // player launch condition:
    // 1) player's velocity is small
    if (!m_pPlayer->isMoving()) {
        // freeze all map elements
        QIXCocos2dHelper::pauseMapElementsOfCurrentScene();
        // catch the touch
        m_bIsLaunching = true;
        return true;
    } else {
        // show ban
        this->showBan(position);
        return false;
    }
}

void PlayerLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (m_bIsLaunchCanceled) {
        m_pArrowSprite->setVisible(false);
        return;
    }
    
    CCPoint position = this->locationFromTouch(touch);
    
    // show the arrow
    m_pArrowSprite->setPosition(m_pPlayer->getPosition());
    CCPoint path = ccpSub(m_vTouchPoints.first, position);
    m_pArrowSprite->setRotation(0.0f);
    // scale the length of the arrow
    float scaleValue = MIN(QIXResourceInstance.m_oGlobalConfig.PlayerArrowLengthMax, ccpLength(path)) / QIXResourceInstance.m_oGlobalConfig.PlayerArrowLengthDefault;
    scaleValue = scaleValue * m_pArrowSprite->getScaleY();
    m_pArrowSprite->setScaleX(scaleValue);
    m_pArrowSprite->setRotation(CC_RADIANS_TO_DEGREES(ccpToAngle(path)*(-1)));
    m_pArrowSprite->setVisible(true);
}

void PlayerLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (!m_bIsLaunchCanceled) {
        CCPoint position = this->locationFromTouch(touch);
        m_vTouchPoints.second = position;
        
        // calculate the player's launch velocity
        CCPoint velocity = ccpSub(m_vTouchPoints.first, m_vTouchPoints.second);
        float ratio = QIXResourceInstance.m_oGlobalConfig.PlayerLaunchVelocityMax / QIXResourceInstance.m_oGlobalConfig.PlayerArrowLengthMax;
        velocity = velocity * ratio;
        float length = ccpLength(velocity);
        if (length > QIXResourceInstance.m_oGlobalConfig.PlayerLaunchVelocityMax) {
            float scale = length/QIXResourceInstance.m_oGlobalConfig.PlayerLaunchVelocityMax;
            velocity.x /= scale;
            velocity.y /= scale;
        } else if (length < QIXResourceInstance.m_oGlobalConfig.PlayerLaunchVelocityMin) {
            velocity = CCPointZero;
        }
        m_pPlayer->setVelocity(velocity);
    }
    
    // un-freeze all map elements
    QIXCocos2dHelper::resumeMapElementsOfCurrentScene();
    // reset state
    m_pArrowSprite->setVisible(false);
    
    if (!m_bIsLaunchCanceled && m_pPlayer->isMoving()) {
        // play launch sound
        QIXAudioEngineInstance.playPlayerLaunch();
        // update game status
        GameScene* scene = static_cast<GameScene*>(this->getParent());
        GameStatusInstance.increaseLaunchCount();
        int display = QIXResourceInstance.m_oLevelConfig.PassCondition.LaunchCountMax - GameStatusInstance.getLaunchCount();
        scene->getPanelLayer()->setLaunchCount(display);
    }
    
    // reset state
    m_bIsLaunchCanceled = false;
    m_bIsLaunching = false;
}

void PlayerLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

CCPoint PlayerLayer::locationFromTouch(CCTouch* touch)
{
    return CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
}
