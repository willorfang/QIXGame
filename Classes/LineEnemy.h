//
//  LineEnemy.h
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#ifndef __QIXGame__LineEnemy__
#define __QIXGame__LineEnemy__

#include "Enemy.h"

/**
 @brief Defines a LineEnemy
 @class LineEnemy
 
 Defines a LineEnemy, whose moving route is line,
 and is always moving with constant speed
 */

typedef enum {
    kLineEnemyDirectionTypeToLeft = 0,
    kLineEnemyDirectionTypeToRight,
    kLineEnemyDirectionTypeToTop,
    kLineEnemyDirectionTypeToBottom,
    kLineEnemyDirectionTypeUnknown
} LineEnemyDirectionType;

//! line enemy role name
extern const char* g_sLineEnemyRoleName[];

//! get direction type from name
LineEnemyDirectionType getLineEnemyDirectionTypeFromName(const char* name);

class LineEnemy : public Enemy {
public:
    //! create an instance
    static LineEnemy* create(const MapElementConfig& config);
    
    //! turn back
    void turnBack();
    
    //! callback when conllided with some object
    virtual void onCollisionWithAnotherObject(B2Node* node);
    
    //! direction type
    CC_SYNTHESIZE_READONLY(LineEnemyDirectionType, m_tDirectionType, DirectionType);
public:
    void setDirectionType(LineEnemyDirectionType type);
    
protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    LineEnemy();
    //! destruct
    virtual ~LineEnemy();
    
private:
    float m_fVelocity;
};

#endif /* defined(__QIXGame__LineEnemy__) */
