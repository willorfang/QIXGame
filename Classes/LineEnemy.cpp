//
//  LineEnemy.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#include "LineEnemy.h"

USING_NS_CC;

const char* g_sLineEnemyRoleName[] = {
    "LineEnemyToLeft",
    "LineEnemyToRight",
    "LineEnemyToTop",
    "LineEnemyToBottom"
};

//! get direction type from name
LineEnemyDirectionType getLineEnemyDirectionTypeFromName(const char* name) {
    for (int i = 0; i < sizeof(g_sLineEnemyRoleName)/sizeof(const char*); i++) {
        if (0 == strcmp(name, g_sLineEnemyRoleName[i])) {
            return static_cast<LineEnemyDirectionType>(i);
        }
    }
    return kLineEnemyDirectionTypeUnknown;
}

LineEnemy* LineEnemy::create(const MapElementConfig& config) {
    LineEnemy* instance = new LineEnemy();
    if (NULL != instance && instance->init(config)) {
        instance->autorelease();
        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

bool LineEnemy::init(const MapElementConfig& config) {
    if (Enemy::init(config)) {
        // default setting
        m_tDirectionType = kLineEnemyDirectionTypeToTop;
        
        this->setBodyType(b2_dynamicBody);
        this->setRole(kRoleTypeLineEnemy);
        // normal mass & no friction & full collision & no damping
        this->setPhysicalParameter(10.0f, 0.0f, 1.0f);
        // set as sensor
        this->setSensor(true);
        // set speed
        m_fVelocity = config.Velocity;
        this->setDirectionType(m_tDirectionType);
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        return true;
    }
    return false;
}

void LineEnemy::setDirectionType(LineEnemyDirectionType type) {
    m_tDirectionType = type;
    CCPoint velocity;
    switch (m_tDirectionType) {
        case kLineEnemyDirectionTypeToLeft:
            velocity = ccp((-1)*m_fVelocity, 0.0f);
            break;
        case kLineEnemyDirectionTypeToRight:
            velocity = ccp(m_fVelocity, 0.0f);
            break;
        case kLineEnemyDirectionTypeToTop:
            velocity = ccp(0.0f, m_fVelocity);
            break;
        case kLineEnemyDirectionTypeToBottom:
            velocity = ccp(0.0f, (-1)*m_fVelocity);
            break;
        default:
            break;
    }
    this->setVelocity(velocity);
}

LineEnemy::LineEnemy() {
    
}

LineEnemy::~LineEnemy() {
    
}

void LineEnemy::turnBack() {
    this->setVelocity(ccpMult(this->getVelocity(), -1));
}

void LineEnemy::onCollisionWithAnotherObject(B2Node* node) {
    switch (node->getRole()) {
        case kRoleTypeMapArea:
        case kRoleTypeWallGrid:
            this->turnBack();
            break;
            
        case kRoleTypeHoleGrid:
            this->beAttracted(node->getPosition());
            break;
            
        default:
            break;
    }
}


