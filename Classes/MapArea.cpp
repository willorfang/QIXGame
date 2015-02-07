//
//  MapArea.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/7/14.
//
//

#include "MapArea.h"
#include "QIXHelper.h"

USING_NS_CC;

MapArea* MapArea::m_pInstance = NULL;

bool MapArea::init() {
    if (B2Edge::init()) {
        // default setting
        m_fInitialAreaSize = AREA_SIZE_ILLEGEL;
        m_iAreaClearRatio = 0;
        this->setRole(kRoleTypeMapArea);
        
        // create clipping node
        CCClippingNode *node = CCClippingNode::create();
        // set up an empty stencil
        m_pStencil = CCNode::create();
        m_pStencil->retain();
        node->setStencil(m_pStencil);
        // add into scene graph tree
        this->setNode(node);
        this->addChild(node);
        
        return true;
    }
    return  false;
}

MapArea::MapArea() {
}

MapArea::~MapArea() {
    CC_SAFE_RELEASE_NULL(m_pStencil);
    // avoid dangling pointer
    m_pInstance = NULL;
}

bool MapArea::setBoundaryArea(const QIXPolygon& origin) {
    CCAssert(origin.size() >= 3, "Invalid boundary pointers");

    // update the clipping area
    if (this->updateClippingArea(origin)) {
        // set initial size
        if (m_fInitialAreaSize == AREA_SIZE_ILLEGEL) {
            m_fInitialAreaSize = QIXGeometry::getPolygonAreaSize(origin);
            CCLOG("Initial area size = %d", static_cast<int>(m_fInitialAreaSize));
        }
        // set the boundary
        B2Edge::setBoundaryArea(origin);
        return true;
    }
    return false;
}

bool MapArea::updateClippingArea(const QIXPolygon& origin) {
    // partition polygon
    std::vector< QIXPolygon > stencils;
    bool partitionSucceeded = QIXGeometry::partitionPolygonIntoConvexParts(origin, stencils);
    if (!partitionSucceeded) {
        return false;
    }
    
    // clear stencil setting
    m_pStencil->removeAllChildren();
    // add stencil setting
    for (int i=0; i<stencils.size(); ++i) {
        // create area-defining object
        CCDrawNode* drawNode = CCDrawNode::create();
        // retain the draw node to avoid auto release
        ccColor4F yellow = {1, 1, 0, 1};
        CCPoint* pointsArray = &(stencils[i][0]);
        int size = stencils[i].size();
        // set area polygon
        drawNode->drawPolygon(pointsArray, size, yellow, 0, yellow);
        m_pStencil->addChild(drawNode);
    }
    
    return true;
}

int MapArea::updateBoundaryPoints(QIXPointSet& path){
    // get boundary points
    const QIXPolygon& boundary = this->getBoundaryPoints();
//    CCLOG("Start to update the map area's boundary");

    // simplify path firstly
    QIXGeometry::simplifyLineSegments(path);
    
    /**
     detect and filter illegal cases
     */
    // 1) lineSegments are intersected by itself
    // 2) lineSegments are intersected with polygon (except for the first & last point)
    // 3) lineSegments' points are near to polygon sides (except for the first & last point)
    if (QIXGeometry::isLineSegmentsIntersected(path)
        || QIXGeometry::isPathPointCloseToPolygon(boundary, path)
        || QIXGeometry::isPointSetCloseToLineSegments(path, boundary)) {
        CCLOG("the path is intersected");
        return UNSIGNED_INT_VALUE_ILLEGAL;
    }
    
    std::vector< QIXPolygon > result;
    bool flag = QIXGeometry::partitionPolygonByLineSegments(boundary, path, result);
    if (!flag) {
        return UNSIGNED_INT_VALUE_ILLEGAL;
    }
    
    // get the bigger area as the new boundary
    float result0Size = QIXGeometry::getPolygonAreaSize(result[0]);
    float result1Size = QIXGeometry::getPolygonAreaSize(result[1]);
    float clearRatio = 0.0f;    
    // get max
    float maxSize = result0Size>result1Size ? result0Size : result1Size;
    QIXPolygon& polygon = result0Size>result1Size ? result[0] : result[1];
    // set boundary
    bool succeed = this->setBoundaryArea(polygon);
    if (!succeed) {
        return UNSIGNED_INT_VALUE_ILLEGAL;
    }
    clearRatio = 1 - maxSize/m_fInitialAreaSize;
    
    // update game status
    int areaClearRatioNew = static_cast<int>(clearRatio*100);
    int areaClearedRatio = areaClearRatioNew - m_iAreaClearRatio;
    m_iAreaClearRatio = areaClearRatioNew;
    CCLOG("Clear ratio = %d%%", m_iAreaClearRatio);
    return areaClearedRatio;
}

bool MapArea::isAreaClearFinished() {
    return m_iAreaClearRatio >= QIXResourceInstance.m_oLevelConfig.PassCondition.AreaClearRatio;
}
