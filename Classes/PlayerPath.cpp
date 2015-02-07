//
//  PlayerPath.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#include "PlayerPath.h"
#include "Player.h"

USING_NS_CC;

bool PlayerPath::init() {
    if (CCNode::init()) {
        // default setting
        m_bIsLastPathPointAdded = false;
        return true;
    }
    return false;
}

PlayerPath::PlayerPath() {
    
}

PlayerPath::~PlayerPath() {
    
}

void PlayerPath::addFirstPathPoint(cocos2d::CCPoint point) {
    m_vPathPoints.clear();
    m_bIsLastPathPointAdded = false;
    this->addPathPoint(point);
}

void PlayerPath::addMiddlePathPoint(cocos2d::CCPoint point) {
    if (false == this->isPathPointsEmpty()
        && false == m_bIsLastPathPointAdded) {
        this->addPathPoint(point);
        // run step effect
        Player* player = static_cast<Player*>(this->getParent());
        CCAssert(NULL != player, "");
        player->runStepEffect();
    }
}

void PlayerPath::addLastPathPoint(cocos2d::CCPoint point) {
    this->addPathPoint(point);
    m_bIsLastPathPointAdded = true;
}