//
//  StaticEnemy.h
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#ifndef __QIXGame__StaticEnemy__
#define __QIXGame__StaticEnemy__

#include "Enemy.h"

/**
 @brief Defines a StaticEnemy
 @class StaticEnemy
 
 Defines a StaticEnemy, who is always static
 */
class StaticEnemy : public Enemy {
public:
    //! create an instance
    static StaticEnemy* create(const MapElementConfig& config);
protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    StaticEnemy();
    //! destruct
    virtual ~StaticEnemy();
};

#endif /* defined(__QIXGame__StaticEnemy__) */
