//
//  Grid.h
//  QIXGame
//
//  Created by Wei Fang on 4/16/14.
//
//

#ifndef __QIXGame__Grid__
#define __QIXGame__Grid__

#include "B2Sprite.h"

/**
 @brief Defines a Grid
 @class Grid
 
 Defines a Grid
 */
class Grid: public B2Sprite {
protected:
    Grid() {
        
    }
    
    virtual ~Grid() {
        
    }
    
    virtual bool init (const MapElementConfig& config) {
        if(B2Sprite::init(config)) {
            return true;
        }
        return false;
    }
};

#endif /* defined(__QIXGame__Grid__) */
