//
//  MapArea.h
//  QIXGame
//
//  Created by Wei Fang on 4/7/14.
//
//

#ifndef __QIXGame__MapArea__
#define __QIXGame__MapArea__

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "B2Edge.h"
#include <vector>

/**
 @brief Defines an area which is the activity area of map elements
 @class MapArea
 
 Defines an area which is the activity area of map elements.
 The map elements should be its children to make a clip effect.
 */
class MapArea : public B2Edge {
public:
    //! create the singleton
    SINGLETON_FUNC(MapArea);
    
    //! set area boundary
    virtual bool setBoundaryArea(const QIXPolygon& origin);
    
    //! update boundary points with player's path points
    int updateBoundaryPoints(QIXPointSet& path);
    
    //! update the area of the clip node
    bool updateClippingArea(const QIXPolygon& origin);
    
    //! add as a child to CCClippingNode
    void addClippingChild(cocos2d::CCNode* node, int zOrder) {
        this->getNode()->addChild(node, zOrder);
    }
    
    //! whether the area clear is finished or not
    bool isAreaClearFinished();
    
    //! area clear ratio
    CC_SYNTHESIZE_READONLY(int, m_iAreaClearRatio, AreaClearRatio);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    MapArea();
    //! destruct
    virtual ~MapArea();
    
private:
    //! initial map area size
    float m_fInitialAreaSize;
    //! stencil for CCClippingNode
    cocos2d::CCNode* m_pStencil;
    //! the singleton
    static MapArea* m_pInstance;
};

#endif /* defined(__QIXGame__MapArea__) */
