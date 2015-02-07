//
//  B2Sprite.h
//  QIXGame
//
//  Created by Wei Fang on 3/12/14.
//
//

#ifndef __QIXGame__B2Sprite__
#define __QIXGame__B2Sprite__

#include "cocos2d.h"
#include "B2Node.h"

class MapLayer;
class PlayerLayer;
class PanelLayer;

/**
 @brief Defines an CCSprite object in the Box2D physical World
 @class B2Sprite
 
 Defines a CCSprite object in the Box2D physical World.
 */
class B2Sprite : public B2Node {
public:
    /** set/get attributes fot both CCNode and b2Body
     */
    //! sprite velocity
    void setVelocity(cocos2d::CCPoint v) {
        m_vVelocity = ccp(v.x, v.y);
        if (NULL != m_pB2Body) {
            m_pB2Body->SetLinearVelocity(b2Vec2(v.x/PTM_RADIO, v.y/PTM_RADIO));
        }
    }
    const cocos2d::CCPoint getVelocity() {
        if (NULL != m_pB2Body) {
            b2Vec2 v = m_pB2Body->GetLinearVelocity();
            m_vVelocity = ccp(v.x * PTM_RADIO, v.y * PTM_RADIO);
        }
        return m_vVelocity;
    }
    
    //! sprite angular velocity
    void setAngularVelocity(float v) {
        m_fAngularVelocity = v;
        if (NULL != m_pB2Body) {
            m_pB2Body->SetAngularVelocity(v);
        }
    }
    float getAngularVelocity() {
        if (NULL != m_pB2Body) {
            m_fAngularVelocity = m_pB2Body->GetAngularVelocity();
        }
        return m_fAngularVelocity;
    }
    
    //! position
    void setPosition(cocos2d::CCPoint v) {
        CCNode::setPosition(v);
        if (NULL != m_pB2Body) {
            m_pB2Body->SetTransform(b2Vec2(v.x/PTM_RADIO, v.y/PTM_RADIO), 0.0f);
        }
    }
    const cocos2d::CCPoint& getPosition() {
        if (NULL != m_pB2Body) {
            b2Vec2 p = m_pB2Body->GetPosition();
            CCNode::setPosition(ccp(p.x*PTM_RADIO, p.y*PTM_RADIO));
        }
        return CCNode::getPosition();
    }
    
    //! rotation
    void setRotation(float rotation) {
        CCNode::setRotation(rotation);
        if (NULL != m_pB2Body) {
            m_pB2Body->SetTransform(m_pB2Body->GetPosition(), CC_DEGREES_TO_RADIANS(rotation)*(-1));
        }
    }
    float getRotation() {
        if (NULL != m_pB2Body) {
            float rotation = -1 * CC_RADIANS_TO_DEGREES(m_pB2Body->GetAngle());
            CCNode::setRotation(rotation);
        }
        return CCNode::getRotation();
    }
    
    //! the default callback when it is killed
    virtual void beKilled();
    
    //! callback when it is attracted by Black Hole
    virtual void beAttracted(const cocos2d::CCPoint& point) {
    }
    
    //! whether it can be attracted or not
    virtual bool canBeAttracted() {
        return false;
    }
    
    //! whether the object is moving or not
    bool isMoving() {
        return ccpLength(this->getVelocity()) >= QIXResourceInstance.m_oGlobalConfig.SpriteVelocityMin;
    }
    
    //! update object's velocity
    void updateVelocity(float ratio = 1.0f);
    
    //! add into box2d world
    bool addIntoBox2DWorld(cocos2d::CCPoint point);
    //! delete from box2d world
    bool removeFromBox2DWorld();
    
    //! set content size
    void setContentSize(cocos2d::CCSize size);
    
    //! get the layer references
    PanelLayer* getPanelLayer();
    MapLayer* getMapLayer();
    PlayerLayer* getPlayerLayer();
    
    //! will delete flag
    CC_SYNTHESIZE(bool, m_bWillDelete, WillDelete);
    //! whether fix rotation
    CC_SYNTHESIZE(bool, m_bFixedRotation, FixedRotation);
    
protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    B2Sprite();
    //! destruct
    virtual ~B2Sprite();

private:
    //! linear velocity
    cocos2d::CCPoint m_vVelocity;
    //! angular velocity
    float m_fAngularVelocity;
};

#endif /* defined(__QIXGame__B2Sprite__) */
