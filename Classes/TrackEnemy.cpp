//
//  TrackEnemy.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#include "TrackEnemy.h"

USING_NS_CC;

TrackEnemy* TrackEnemy::create(const MapElementConfig& config) {
    TrackEnemy* instance = new TrackEnemy();
    if (NULL != instance && instance->init(config)) {
        instance->autorelease();
        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

bool TrackEnemy::init(const MapElementConfig& config) {
    if (Enemy::init(config)) {
        this->setBodyType(b2_dynamicBody);
        this->setRole(kRoleTypeTrackEnemy);
        // normal mass & small friction & small collision & add damping
        this->setPhysicalParameter(10.0f, 0.3f, 0.3f, 1.0f, 1.0f);
        this->setVelocity(CCPointZero);
        // init track area and speed
        m_fRadius = config.Radius;
        m_fVelocity = config.Velocity;
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        // register update
        this->scheduleUpdate();
        return true;
    }
    return false;
}

TrackEnemy::TrackEnemy() {
    
}

TrackEnemy::~TrackEnemy() {
    
}

void TrackEnemy::update(float dt) {
    // get the track direction
    CCPoint direction = ccpNormalize(ccpSub(m_pTarget->getPosition(), this->getPosition()));
    float velocity = 0;
    
    if (this->isMoving()) {
        this->updateVelocity(0.3f);
        velocity = ccpLength(this->getVelocity());
    } else if (NULL != m_pTarget
        && m_pTarget->canBeAttracted()
        && ccpDistance(m_pTarget->getPosition(), this->getPosition()) <= m_fRadius) {
        velocity = m_fVelocity;
    }
    
    // set velocity
    if (0 != velocity) {
        this->setVelocity(ccpMult(direction, velocity));
    }
}

void TrackEnemy::beAttracted(const cocos2d::CCPoint& point) {
    // leave from b2World
    this->removeFromBox2DWorld();
    
    // move to the black hole
    CCMoveTo* moveTo = CCMoveTo::create(0.3, point);
    CCScaleTo* scaleTo = CCScaleTo::create(0.3, 0.5);
    CCFadeOut* fadeOut = CCFadeOut::create(0.3);
    CCSpawn* uniteAction = CCSpawn::create(moveTo, scaleTo, fadeOut, NULL);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(TrackEnemy::missCallFunc));
    CCFiniteTimeAction* seq = CCSequence::create(uniteAction, callFunc, NULL);
    this->runAction(seq);
}

void TrackEnemy::missCallFunc() {
    this->setWillDelete(true);
}

void TrackEnemy::onCollisionWithAnotherObject(B2Node* node) {
    switch (node->getRole()) {            
        case kRoleTypeHoleGrid:
            this->beAttracted(node->getPosition());
            break;
            
        default:
            break;
    }
}
