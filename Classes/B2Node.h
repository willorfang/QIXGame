//
//  B2Node.h
//  QIXGame
//
//  Created by Wei Fang on 4/8/14.
//
//

#ifndef __QIXGame__B2Node__
#define __QIXGame__B2Node__

#include "Box2D/Box2D.h"
#include "cocos2d.h"

#include "QIXHelper.h"

typedef struct {
    float density;
    float friction;
    float restitution;
    float linerDamping;
    float angularDamping;
} PhysicalParameter;

/**
 @brief Defines an CCNode object in the Box2D physical World
 @class B2Node
 
 Defines an CCNode object in the Box2D physical World.
 Its movement can be simulated with Box2D functionalities.
 */
class B2Node : public cocos2d::CCNode {
public:
    //! create an instance
    CREATE_FUNC(B2Node);
    
    //! b2Body physical parameters
    /** ATTETION: should be set before add into box2d world
     */
    void setPhysicalParameter(float desity = 2.0f,
                              float friction = 0.4f,
                              float restition = 0.7f,
                              float linerDamping = 0.0f,
                              float angularDamping = 0.01f) {
        m_sPhysicalParameter.density = desity;
        m_sPhysicalParameter.friction = friction;
        m_sPhysicalParameter.restitution = restition;
        m_sPhysicalParameter.linerDamping = linerDamping;
        m_sPhysicalParameter.angularDamping = angularDamping;
    }
    const PhysicalParameter& getPhysicalParameter() const {
        return m_sPhysicalParameter;
    }
    
    //! get the object's CCRect area
    cocos2d::CCRect getContentRectArea(float scaleRatio = 1.0f) {
        // get absolute coordinate
        cocos2d::CCPoint origin = this->convertToWorldSpace(m_pNode->getPosition());
        cocos2d::CCSize size = this->getContentSize();
        return cocos2d::CCRectMake(origin.x-size.width*scaleRatio*0.5,
                                   origin.y-size.height*scaleRatio*0.5,
                                   size.width*scaleRatio,
                                   size.height*scaleRatio);
    }
    
    //! get role name
    const char * getRoleName() {
        CCAssert(m_uRole < sizeof(g_sRoleName)/sizeof(char*), "Invalid role type");
        return g_sRoleName[m_uRole];
    }
    
    //! judge the role type
    bool isWorldEdge() {
        return m_uRole == kRoleTypeWorldEdge;
    }
    bool isMapArea() {
        return m_uRole == kRoleTypeMapArea;
    }
    bool isEnemy() {
        return m_uRole == kRoleTypeStaticEnemy || m_uRole == kRoleTypeTrackEnemy || m_uRole == kRoleTypeLineEnemy;
    }
    bool isPlayer() {
        return m_uRole == kRoleTypePlayer;
    }
    bool isGrid() {
        return m_uRole == kRoleTypeHoleGrid || m_uRole == kRoleTypeWallGrid;
    }
    
    //! callback when conllided with some object
    /** It should be re-implemented by its child class for different responses if has any
     */
    virtual void onCollisionWithAnotherObject(B2Node* node) {
    }
    
    //! whether the object can response to collision or not
    /** It should be re-implemented by its child class for different responses if has any
     */
    virtual bool canResponseToCollision() {
        return true;
    }
    
    //! the related b2Body in the b2World
    /** it will be created when added into box2d world
     */
    CC_SYNTHESIZE(b2Body*, m_pB2Body, B2Body);
    
    //! the related CCNode object
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pNode, Node);
    
    //! body type of the b2Body element
    /** ATTETION: should be set before add into box2d world
     */
    CC_SYNTHESIZE(b2BodyType, m_uBodyType, BodyType);
    
    //! role type of the object
    CC_SYNTHESIZE(RoleType, m_uRole, Role);
    
    //! only detect collision when m_bSensor is true
    CC_SYNTHESIZE(bool, m_bSensor, Sensor);
    
protected:
    //! initialize
    virtual bool init() {
        if (cocos2d::CCNode::init()) {
            m_bSensor = false;
            this->setPhysicalParameter();
            m_pB2Body = NULL;
            m_pNode = NULL;
            m_uBodyType = b2_staticBody;
            m_uRole = kRoleTypeDefault;
            return true;
        }
        return false;
    }
    
    //! construct
    B2Node() {
        
    }
    
    //! destruct
    virtual ~B2Node() {
        
    }
    
private:
    //! physical parameter struct
    PhysicalParameter m_sPhysicalParameter;
};

#endif /* defined(__QIXGame__B2Node__) */
