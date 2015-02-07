//
//  Enemy.h
//  QIXGame
//
//  Created by Wei Fang on 4/4/14.
//
//

#ifndef __QIXGame__Enemy__
#define __QIXGame__Enemy__

#include "B2Sprite.h"

/**
 @brief Defines an enemy
 @class Enemy
 
 Defines an enemy
 */
class Enemy: public B2Sprite {
protected:
    Enemy() {
        
    }
    
    virtual ~Enemy() {
        
    }
    
    virtual bool init (const MapElementConfig& config) {
        if(B2Sprite::init(config)) {
            return true;
        }
        return false;
    }
};

#endif /* defined(__QIXGame__Enemy__) */
