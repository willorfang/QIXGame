//
//  B2Edge.h
//  QIXGame
//
//  Created by Wei Fang on 4/16/14.
//
//

#ifndef __QIXGame__B2Edge__
#define __QIXGame__B2Edge__

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "B2Node.h"
#include <vector>

/**
 @brief Defines a closed edge collection
 @class B2Edge
 
 Defines a closed edge collection
 */
class B2Edge : public B2Node {
public:
    //! create an instance
    CREATE_FUNC(B2Edge);
    
    //! set area boundary
    virtual bool setBoundaryArea(const QIXPolygon& origin);
    
    //! whether contains a point or not
    bool containsPoint(cocos2d::CCPoint point);
    
    //! get intersection point with a line segment
    //! return CCPointZero if there is no intersection point
    cocos2d::CCPoint getInterSectionPoint(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB);
    
    //! add into box2d world
    bool addIntoBox2DWorld(const QIXPolygon& origin);
    //! delete from box2d world
    bool removeFromBox2DWorld();
    
    const QIXPolygon& getBoundaryPoints() {
        return m_vBoundaryPoints;
    }
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    B2Edge();
    //! destruct
    virtual ~B2Edge();
    
private:
    //! one shoule add boundary points clockwisely
    QIXPolygon m_vBoundaryPoints;
};


#endif /* defined(__QIXGame__B2Edge__) */
