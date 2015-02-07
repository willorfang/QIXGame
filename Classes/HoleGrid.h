//
//  HoleGrid.h
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#ifndef __QIXGame__HoleGrid__
#define __QIXGame__HoleGrid__

#include "Grid.h"

/**
 @brief Defines a HoleGrid
 @class HoleGrid
 
 Defines a HoleGrid, who will attract anything that collide with it
 */
class HoleGrid : public Grid {
public:
    //! create an instance
    static HoleGrid* create(const MapElementConfig& config);
    //! callback when conllided with some object
    virtual void onCollisionWithAnotherObject(B2Node* node);
    //! animation when catch something
    void runAttractAction();

protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    HoleGrid();
    //! destruct
    virtual ~HoleGrid();
};

#endif /* defined(__QIXGame__HoleGrid__) */
