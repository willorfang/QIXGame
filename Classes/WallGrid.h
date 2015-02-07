//
//  WallGrid.h
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#ifndef __QIXGame__WallGrid__
#define __QIXGame__WallGrid__

#include "Grid.h"

/**
 @brief Defines a WallGrid
 @class WallGrid
 
 Defines a WallGrid, who is always static
 */
class WallGrid : public Grid {
public:
    //! create an instance
    static WallGrid* create(const MapElementConfig& config);
    
protected:
    //! initialize
    virtual bool init(const MapElementConfig& config);
    //! construct
    WallGrid();
    //! destruct
    virtual ~WallGrid();
};

#endif /* defined(__QIXGame__WallGrid__) */
