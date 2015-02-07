//
//  TrackEnemy.h
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#ifndef __QIXGame__TrackEnemy__
#define __QIXGame__TrackEnemy__

#include "Enemy.h"

/**
 @brief Defines a TrackEnemy
 @class TrackEnemy
 
 Defines a TrackEnemy, who is static when no target is in its scope.
 It will move towards the target when a target is in its scope
 */
class TrackEnemy : public Enemy {
public:
    //! create an instance
    static TrackEnemy* create(const MapElementConfig& config);
    //! callback when it is attracted by Black Hole
    virtual void beAttracted(const cocos2d::CCPoint& point);
    //! callback when conllided with some object
    virtual void onCollisionWithAnotherObject(B2Node* node);
    //! the track target
    CC_SYNTHESIZE(B2Sprite*, m_pTarget, Target);

protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    TrackEnemy();
    //! destruct
    virtual ~TrackEnemy();
    
    //! to check track condition, and track the traget
    virtual void update(float dt);
    //! callback for beAttracted()
    void missCallFunc();
    
private:
    //! track area radius
    float m_fRadius;
    //! track speed
    float m_fVelocity;
};

#endif /* defined(__QIXGame__TrackEnemy__) */
