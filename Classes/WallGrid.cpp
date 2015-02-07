//
//  WallGrid.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#include "WallGrid.h"

USING_NS_CC;

WallGrid* WallGrid::create(const MapElementConfig& config) {
    WallGrid* instance = new WallGrid();
    if (NULL != instance && instance->init(config)) {
        instance->autorelease();
        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

bool WallGrid::init(const MapElementConfig& config) {
    if (Grid::init(config)) {
        this->setBodyType(b2_staticBody);
        this->setRole(kRoleTypeWallGrid);
        this->setVelocity(CCPointZero);
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        return true;
    }
    return false;
}

WallGrid::WallGrid() {
    
}

WallGrid::~WallGrid() {
    
}