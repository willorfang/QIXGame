//
//  StaticEnemy.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#include "StaticEnemy.h"

USING_NS_CC;

StaticEnemy* StaticEnemy::create(const MapElementConfig& config) {
    StaticEnemy* instance = new StaticEnemy();
    if (NULL != instance && instance->init(config)) {
        instance->autorelease();
        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

bool StaticEnemy::init(const MapElementConfig& config) {
    if (Enemy::init(config)) {
        this->setBodyType(b2_staticBody);
        this->setRole(kRoleTypeStaticEnemy);
        this->setVelocity(CCPointZero);
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        return true;
    }
    return false;
}

StaticEnemy::StaticEnemy() {
    
}

StaticEnemy::~StaticEnemy() {
    
}