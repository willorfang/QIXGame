//
//  B2Sprite.cpp
//  QIXGame
//
//  Created by Wei Fang on 3/12/14.
//
//

#include "B2Sprite.h"
#include "Box2dWorld.h"
#include "PlayerLayer.h"
#include "PanelLayer.h"
#include "MapLayer.h"
#include "GameScene.h"
#include "QIXHelper.h"

USING_NS_CC;

bool B2Sprite::init(const MapElementConfig& config) {
    if (B2Node::init()) {
        // default setting
        m_vVelocity = CCPointZero;
        m_fAngularVelocity = 0.0f;
        m_bWillDelete = false;
        m_bFixedRotation = false;
        
        // create CCSprite object
        CCSprite* sprite = QIXCocos2dHelper::createSprite(config.Image.c_str());
        // add CCSprite into scene graph tree
        this->setNode(sprite);
        this->addChild(this->getNode());
        // set size
        this->setContentSize(config.Size);
        // set position
        cocos2d::CCPoint pos = config.Position;
        QIXLayout::getAbsolutePositionWithScreenRect(pos);
        this->setPosition(pos);
        
        return true;
    }
    return false;
}

B2Sprite::B2Sprite() {
    
}

B2Sprite::~B2Sprite() {
    
}

void B2Sprite::setContentSize(cocos2d::CCSize size) {
    // set CCNode content size
    B2Node::setContentSize(size);
    // set physical body size
    Box2DWorld::setPhysicalBodySize(this, MAX(size.width, size.height));
    
    // set CCNode's attributes
    CCNode* node = this->getNode();
    CCRect rect = CCRectMake(size.width*(-0.5), size.height*(-0.5), size.width, size.height);
    QIXCocos2dHelper::setContentRect(node, rect, kScaleShowAll);
}

void B2Sprite::beKilled() {
    Box2DWorld::GetTheBox2DWorld()->removeBody(this);
    this->removeFromParentAndCleanup(true);
    
    // send message
    CCLOG("Send message {%s}", CCNC_MSG_ELEMENT_KILLED);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(CCNC_MSG_ELEMENT_KILLED, this);
}

bool B2Sprite::addIntoBox2DWorld(CCPoint point) {
    this->setPosition(point);
    return Box2DWorld::GetTheBox2DWorld()->addSpriteBody(this);
}

bool B2Sprite::removeFromBox2DWorld() {
    return Box2DWorld::GetTheBox2DWorld()->removeBody(this);
}

PanelLayer* B2Sprite::getPanelLayer() {
    GameScene* gameScene = static_cast<GameScene*>(this->getParent()->getParent());
    CCAssert(NULL != dynamic_cast<GameScene*>(gameScene), "");
    return gameScene->getPanelLayer();
}

MapLayer* B2Sprite::getMapLayer() {
    GameScene* gameScene = static_cast<GameScene*>(this->getParent()->getParent());
    CCAssert(NULL != dynamic_cast<GameScene*>(gameScene), "");
    return gameScene->getMapLayer();
}

PlayerLayer* B2Sprite::getPlayerLayer() {
    GameScene* gameScene = static_cast<GameScene*>(this->getParent()->getParent());
    CCAssert(NULL != dynamic_cast<GameScene*>(gameScene), "");
    return gameScene->getPlayerLayer();
}

void B2Sprite::updateVelocity(float ratio) {
    if (this->isMoving()) {
        CCPoint originVec = this->getVelocity();
        float factor = QIXResourceInstance.m_oGlobalConfig.SpriteVelocityControlFactor * ratio;
        CCPoint dtVec = ccpNormalize(originVec) * factor;
        if (ccpLength(originVec) > ccpLength(dtVec)) {
            this->setVelocity(originVec - dtVec);
        } else {
            this->setVelocity(CCPointZero);
        }
    }
}