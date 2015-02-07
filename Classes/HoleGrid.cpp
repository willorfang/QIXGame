//
//  HoleGrid.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#include "HoleGrid.h"

USING_NS_CC;

HoleGrid* HoleGrid::create(const MapElementConfig& config) {
    HoleGrid* instance = new HoleGrid();
    if (NULL != instance && instance->init(config)) {
        instance->autorelease();
        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

bool HoleGrid::init(const MapElementConfig& config) {
    if (Grid::init(config)) {
        this->setBodyType(b2_staticBody);
        this->setRole(kRoleTypeHoleGrid);
        this->setVelocity(CCPointZero);
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        return true;
    }
    return false;
}

HoleGrid::HoleGrid() {
    
}

HoleGrid::~HoleGrid() {
    
}

void HoleGrid::runAttractAction() {
    CCScaleTo *scaleTo = CCScaleTo::create(0.3, 1.5);
    CCScaleTo *scaleback = CCScaleTo::create(0.3, 1.0);
    CCFiniteTimeAction *seq = CCSequence::create(scaleTo, scaleback, NULL);
    this->runAction(seq);
}

void HoleGrid::onCollisionWithAnotherObject(B2Node* node) {
    this->runAttractAction();
}