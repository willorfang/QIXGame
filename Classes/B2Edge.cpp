//
//  B2Edge.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/16/14.
//
//

#include "B2Edge.h"
#include "Box2dWorld.h"

USING_NS_CC;

bool B2Edge::init() {
    if (B2Node::init()) {
        // default setting
        this->setBodyType(b2_staticBody);
        return true;
    }
    return  false;
}

B2Edge::B2Edge() {
    
}

B2Edge::~B2Edge() {
    
}

bool B2Edge::setBoundaryArea(const QIXPolygon& origin) {
    CCAssert(origin.size() >= 2, "");
    
    // update the boundary
    m_vBoundaryPoints = origin;
    
    // reset the map area in Box2D world
    this->removeFromBox2DWorld();
    this->addIntoBox2DWorld(m_vBoundaryPoints);
    
    return true;
}

cocos2d::CCPoint B2Edge::getInterSectionPoint(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB) {
    for (int i=0; i<m_vBoundaryPoints.size(); i++) {
        CCPoint pointStart = m_vBoundaryPoints[i];
        int index_next = (i+1) % m_vBoundaryPoints.size();
        CCPoint pointEnd = m_vBoundaryPoints[index_next];
        if (ccpSegmentIntersect(pointA, pointB, pointStart, pointEnd)) {
            return ccpIntersectPoint(pointA, pointB, pointStart, pointEnd);
        }
    }
    return CCPointZero;
}

bool B2Edge::addIntoBox2DWorld(const QIXPolygon& origin) {
    return Box2DWorld::GetTheBox2DWorld()->addEdgeBody(this, origin);
}

bool B2Edge::removeFromBox2DWorld() {
    return Box2DWorld::GetTheBox2DWorld()->removeBody(this);
}

bool B2Edge::containsPoint(cocos2d::CCPoint point) {
    return QIXGeometry::polygonContainsPoint(m_vBoundaryPoints, point);
}