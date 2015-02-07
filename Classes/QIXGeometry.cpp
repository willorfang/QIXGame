//
//  QIXGeometry.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#include "QIXGeometry.h"

USING_NS_CC;
using namespace std;

#define LAYER_OUT_DEBUG_VIEW        128

#define FLOAT_ZERO_VALUE            0.1f
#define DISTANCE_NEAR_VALUE         1.0f

/*************************************************************************
 debug draw related helper
 *************************************************************************/

static QIXGeometry QIXGeometryInstance;

QIXGeometry::QIXGeometry() {
    CCLOG("QIXGeometry instance is created");
    // initialize parent draw node for debug draw
    m_pParentDrawNode = CCNode::create();
    // retain the parent node
    m_pParentDrawNode->retain();
    CCAssert(NULL != m_pParentDrawNode, "");
}

QIXGeometry::~QIXGeometry() {
    // release the parent node
    CC_SAFE_RELEASE_NULL(m_pParentDrawNode);
}

/* order: R, G, B, A */
static const ccColor4F colorRed = {1, 0, 0, 1};
static const ccColor4F colorBlue = {0, 0, 1, 1};
static const ccColor4F colorBlack = {0, 0, 0, 1};

static int polygonNormalZValue = 1;
static int segmentPartitionZValue = 10;
static int polygonErrorZValue = 20;
static int pointNormalZValue = 30;
static int pointErrorZValue = 31;

/*************************************/
void QIXGeometry::drawDebugPolygon(const QIXPolygon& origin, ccColor4F color, int zValue) {
    // create draw node
    CCDrawNode* drawNode = CCDrawNode::create();
    ccColor4F fillColor = {1, 0, 0, 0};
    
    CCPoint* pointsArray = const_cast<CCPoint*>(&(origin[0]));
    int size = origin.size();
    // draw area polygon
    drawNode->drawPolygon(pointsArray, size, fillColor, 2.0f, color);
    CCAssert(NULL != m_pParentDrawNode, "");
    m_pParentDrawNode->addChild(drawNode, zValue);
}

void QIXGeometry::drawEdgePolygon(const QIXPolygon& origin) {
    this->drawDebugPolygon(origin, colorBlack, polygonNormalZValue);
}

void QIXGeometry::drawErrorPolygon(const QIXPolygon& origin) {
    this->drawDebugPolygon(origin, colorRed, polygonErrorZValue);
}

/*************************************/
void QIXGeometry::drawDebugPoint(CCPoint point, ccColor4F color, int zValue) {
    // create draw node
    CCDrawNode* drawNode = CCDrawNode::create();
    
    drawNode->drawDot(point, 4.0, color);
    CCAssert(NULL != m_pParentDrawNode, "");
    m_pParentDrawNode->addChild(drawNode, zValue);
}

void QIXGeometry::drawErrorPoint(CCPoint point) {
    this->drawDebugPoint(point, colorRed, pointErrorZValue);
}
void QIXGeometry::drawNormalPoint(cocos2d::CCPoint point) {
    this->drawDebugPoint(point, colorBlack, pointNormalZValue);
}

/*************************************/
void QIXGeometry::drawNormalPartitionSegments(const QIXPointSet& origin) {
    for (int i=0; i<origin.size()-1; i++) {
        this->drawDebugSegment(origin[i], origin[i+1], colorBlue, segmentPartitionZValue);
    }
}

void QIXGeometry::drawErrorPartitionSegments(const QIXPointSet& origin) {
    for (int i=0; i<origin.size()-1; i++) {
        this->drawDebugSegment(origin[i], origin[i+1], colorRed, segmentPartitionZValue);
    }
}

void QIXGeometry::drawDebugSegment(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, cocos2d::ccColor4F color, int zValue) {
    // create draw node
    CCDrawNode* drawNode = CCDrawNode::create();
    
    drawNode->drawSegment(pointA, pointB, 2.0f, color);
    CCAssert(NULL != m_pParentDrawNode, "");
    m_pParentDrawNode->addChild(drawNode, zValue);
}

void QIXGeometry::clearDebugDraw() {
    CCAssert(NULL != m_pParentDrawNode, "");
    m_pParentDrawNode->removeAllChildren();
}

/*************************************************************************
 geometry related algorithms
 *************************************************************************/

bool QIXGeometry::getIntersectionOfLineSegmentAndRay(CCPoint linePointA,
                                                   CCPoint linePointB,
                                                   CCPoint rayPointA,
                                                   CCPoint rayPointB,
                                                   CCPoint& intersection) {
    float linePosition, rayPosition;
    bool flag = ccpLineIntersect(linePointA, linePointB, rayPointA, rayPointB, &linePosition, &rayPosition);
    if (flag
        && linePosition >= 0
        && linePosition <= 1
        && rayPosition >= 0) {
        intersection = rayPointA + (rayPointB - rayPointA) * rayPosition;
        return true;
    }
    return false;
}

CCPoint QIXGeometry::getFirstIntersectionPointWithPolygon(const QIXPolygon& origin,
                                                          int index,
                                                          int& lineStartIndex,
                                                          int& lineEndIndex) {
    int count = origin.size();
    CCAssert(index >= 0 && index < count && count >= 3, "Illegal polygon or index");
    // next index
    int index_next = (index+1) % count;
    
    CCPoint resultPoint = CCPointZero;
    float distanceMin = 100000000.0f;
    for (int i=0; i<count; ++i) {
        int i_prev = (i-1+count) % count;
        int i_next = (i+1) % count;
        // ignore the itself and neighbours
        if (i_prev == index || i == index || i_next == index) {
            continue;
        }
        // get intersection
        CCPoint intersection;
        bool flag = QIXGeometry::getIntersectionOfLineSegmentAndRay(origin[i], origin[i_next],origin[index], origin[index_next], intersection);
        if (flag) {
            float distance = ccpDistance(origin[index_next], intersection);
            if (distance < distanceMin) {
                distanceMin = distance;
                resultPoint = intersection;
                lineStartIndex = i;
                lineEndIndex = i_next;
            }
        }
    }
    
    return resultPoint;
}

bool QIXGeometry::judgePointAndLineSegment(CCPoint lineSegmentPointA,
                                           CCPoint lineSegmentPointB,
                                           CCPoint point,
                                           float distance) {
    CCPoint lineAB = lineSegmentPointB - lineSegmentPointA;
    CCPoint lineAP = point - lineSegmentPointA;
    CCPoint linePB = lineSegmentPointB - point;
    
    return fabs(ccpLength(lineAB) - ccpLength(lineAP) - ccpLength(linePB)) < distance;
}

bool QIXGeometry::isPointInLineSegment(CCPoint lineSegmentPointA,
                                       CCPoint lineSegmentPointB,
                                       CCPoint point) {
    return QIXGeometry::judgePointAndLineSegment(lineSegmentPointA, lineSegmentPointB, point, FLOAT_ZERO_VALUE);
}

bool QIXGeometry::isPointCloseToLineSegment(CCPoint lineSegmentPointA,
                                       CCPoint lineSegmentPointB,
                                       CCPoint point) {
    return QIXGeometry::judgePointAndLineSegment(lineSegmentPointA, lineSegmentPointB, point, DISTANCE_NEAR_VALUE);
}

float QIXGeometry::getPolygonAreaSize(const QIXPolygon& origin) {
    int count = origin.size();
    CCAssert(count >= 3, "Illegal polygon");
    
    float area = 0.0f;
    for (int i=1; i<count-1; ++i) {
        area += (origin[i].x - origin[0].x)*(origin[i+1].y - origin[0].y) - (origin[i+1].x - origin[0].x)*(origin[i].y - origin[0].y);
    }
    
    return fabs(area)*0.5;
}

bool QIXGeometry::polygonContainsPoint(const QIXPolygon& origin, CCPoint point) {
    int count = origin.size();
    CCAssert(count >= 3, "Illegal polygon");
    
    int nCross = 0;
    for (int i=0; i<count; ++i) {
        cocos2d::CCPoint p1 = origin[i];
        cocos2d::CCPoint p2 = origin[(i+1)%count];
        
        if (p1.y == p2.y) {
            continue;
        }
        if (point.y < fminf(p1.y, p2.y)) {
            continue;
        }
        if (point.y > fmaxf(p1.y, p2.y)) {
            continue;
        }
        
        // get the x of intersection
        float intersection_x = (point.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
        if (intersection_x > point.x) {
            nCross++;
        }
    }
    return nCross%2 != 0 ? true : false;
}

bool QIXGeometry::partitionPolygonByLineSegments(const QIXPolygon& origin,
                                               const QIXPointSet& lineSegments,
                                               std::vector< QIXPolygon >& result) {
    return QIXGeometry::partitionPolygonByLineSegments(origin, lineSegments, result, false);
}

bool QIXGeometry::partitionPolygonByLineSegments(const QIXPolygon& origin,
                                               const QIXPointSet& lineSegments,
                                               std::vector< QIXPolygon >& result,
                                               bool callByInternal) {
    CCAssert(origin.size() >= 3 && lineSegments.size() >=2, "Illegal polygon or line segment");
    
    // clear the result firstly
    result.clear();
    
    // get the sides which contains the first and the last point of lineSegments
    // represented by its side verteces
    int index[4] = {-1, -1, -1, -1};
    CCPoint lineStartPoint = lineSegments[0];
    CCPoint lineEndPoint = lineSegments[lineSegments.size()-1];
    
    // get the sides which contains the first and the last point of lineSegments
    // represented by its side verteces
    for (int i=0; i<origin.size(); ++i) {
        CCPoint boundaryStartPoint = origin[i];
        int index_next = (i+1) % origin.size();
        CCPoint boundaryEndPoint = origin[index_next];
        
        // whether it is the line segment which contains intersection point
        if (QIXGeometry::isPointInLineSegment(boundaryStartPoint, boundaryEndPoint, lineStartPoint)) {
            index[0] = i;
            index[1] = index_next;
        }
        if (QIXGeometry::isPointInLineSegment(boundaryStartPoint, boundaryEndPoint, lineEndPoint)) {
            index[2] = i;
            index[3] = index_next;
        }
    }
    
    if(index[0] == -1 || index[1] == -1 || index[2] == -1 || index[3] == -1) {
        CCLOG("the start OR end point of path is NOT on polygon's sides");
        return false;
    }
    
    // the boundary verteces for the two divided areas
    QIXPolygon areaA(lineSegments);
    QIXPolygon areaB(lineSegments);
    reverse(areaB.begin(), areaB.end());
    
    if (index[0] == index[2] && index[1] == index[3]) {
        // if the start and end point is on the same side
        QIXPolygon& temp = areaA;
        if (QIXGeometry::isPointInLineSegment(origin[index[0]], lineEndPoint, lineStartPoint)) {
            // in this case, areaA is clockwise (the same as origin)
            temp = areaA;
        } else {
            // in this case, areaB is clockwise (the same as origin)
            temp = areaB;
        }
        for (int i=0, j=index[1]; i<origin.size(); ++i, j=(j+1)%origin.size()) {
            if (!QIXGeometry::isTheSamePoint(origin[j],lineStartPoint) && !QIXGeometry::isTheSamePoint(origin[j],lineEndPoint)) {
                temp.push_back(origin[j]);
            }
        }
    } else {
        // if the start and end point is NOT on the same side
        // divide origin into areaA
        for (int i = index[3]; i != (index[0]+1)%origin.size(); i = (i+1)%origin.size()) {
            if (!QIXGeometry::isTheSamePoint(origin[i],lineStartPoint) && !QIXGeometry::isTheSamePoint(origin[i],lineEndPoint)) {
                areaA.push_back(origin[i]);
            }
        }
        
        // divide origin into areaB
        for (int i = index[1]; i != (index[2]+1)%origin.size(); i = (i+1)%origin.size()) {
            if (!QIXGeometry::isTheSamePoint(origin[i],lineStartPoint) && !QIXGeometry::isTheSamePoint(origin[i],lineEndPoint)) {
                areaB.push_back(origin[i]);
            }
        }
    }
    
    /**
     when it is called by partitionPolygonIntoConvexParts(),
     delete the concave point from one
     */
    if (callByInternal) {
        areaA.erase(areaA.begin());
    }
    
    // collect the results
    result.push_back(areaA);
    result.push_back(areaB);
    
    return true;
}


bool QIXGeometry::isLineSegmentsIntersected(const QIXPointSet& origin) {
    int count = origin.size();
    if (count < 4) {
        return false;
    }
    
    for (int i=0; i<=count-4; ++i) {
        for (int j=i+2; j<=count-2; ++j) {
            if (ccpSegmentIntersect(origin[i], origin[i+1], origin[j], origin[j+1])) {
                return true;
            }
        }
    }
    
    return false;
}

bool QIXGeometry::isTheSamePoint(CCPoint pointA, CCPoint pointB) {
    return fabsf(pointA.x-pointB.x) < FLOAT_ZERO_VALUE && fabsf(pointA.y-pointB.y) < FLOAT_ZERO_VALUE;
}

bool QIXGeometry::partitionPolygonIntoConvexPartsWithDebugDraw(const QIXPolygon& origin,
                                                             std::vector< QIXPolygon >& result) {
    bool retValue = true;
    
    // initialize if necessary
    CCNode* parentNode = QIXGeometryInstance.getParentDrawNode();
    CCAssert(NULL != parentNode, "");
    
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    if (NULL != scene) {
        /** the current scene has initialized, we can debug draw
         */
        // clear previous debug draw
        QIXGeometryInstance.clearDebugDraw();
        
        // update current scene if necessary
        if(scene != parentNode->getParent()) {
            parentNode->setParent(NULL);
        }
        // add into the current sceneif necessary
        if (NULL == parentNode->getParent()) {
            scene->addChild(parentNode, LAYER_OUT_DEBUG_VIEW);
        }
        
        // draw the first vertex
        QIXGeometryInstance.drawErrorPoint(origin[0]);
        // draw polygon edge
        QIXGeometryInstance.drawEdgePolygon(origin);
        //
        retValue = QIXGeometry::partitionPolygonIntoConvexParts(origin, result, true);
    } else {
        /** the current scene has NOT initialized, we can not debug draw
         */
        retValue = QIXGeometry::partitionPolygonIntoConvexParts(origin, result, false);
    }
    
    return retValue;
}

float QIXGeometry::getDeterminant(CCPoint pointA, CCPoint pointB, CCPoint pointC) {
    double Ax = static_cast<double>(pointA.x);
    double Ay = static_cast<double>(pointA.y);
    double Bx = static_cast<double>(pointB.x);
    double By = static_cast<double>(pointB.y);
    double Cx = static_cast<double>(pointC.x);
    double Cy = static_cast<double>(pointC.y);
    
    double value = Ax*By + Bx*Cy + Cx*Ay - Bx*Ay - Cx*By - Ax*Cy;
    return value;
}

bool QIXGeometry::partitionPolygonIntoConvexParts(const QIXPolygon &origin,
                                                std::vector< QIXPolygon >& result) {
    // clear
    result.clear();
    //
    return QIXGeometry::partitionPolygonIntoConvexParts(origin, result, false);
}

bool QIXGeometry::partitionPolygonIntoConvexParts(const QIXPolygon &origin,
                                                std::vector< QIXPolygon >& result,
                                                bool debug) {
    // whether the polygon is convex or not
    bool isConvex = true;
    QIXPointSet concavePoints;
    
    // check polygon's validity
    int size = origin.size();
    if (size < 3) {
        CCLOG("***** Patition polygon failed: Illegal polygon (vertex count < 3) *****");
        if (debug) {
            QIXGeometry::logPolygonPointers(origin);
            QIXGeometryInstance.drawErrorPartitionSegments(origin);
        }
        return false;
    }
    
    // find concave vertex (if any) and partition
    for (int i=0; i<size; ++i) {
        int prev = (i-1+size) % size;
        int next = (i+1) % size;
        
        float value = getDeterminant(origin[prev], origin[i], origin[next]);
        // value = 0: prev, i, next is on the same line, skip it
        if (fabs(value) < FLOAT_ZERO_VALUE) {
            CCLOG("***** Patition polygon: skip the condition that three verteces are nearly in line *****");
            continue;
        }
        
        // value > 0 : when origin[i] is a concave point
        if (value > FLOAT_ZERO_VALUE) {
            // it is cancave NOW
            isConvex = false;
            concavePoints.push_back(origin[i]);
            
            // line origin[i]--destinationPoint to partition origin
            CCPoint destinationPoint;
            
            // run Rogers algorithm to find destinationPoint
            int lineStartIndex, lineEndIndex;
            destinationPoint = QIXGeometry::getFirstIntersectionPointWithPolygon(origin, prev, lineStartIndex, lineEndIndex);
            // the line segments
            QIXPointSet pointSet;
            pointSet.push_back(origin[i]);
            pointSet.push_back(destinationPoint);
            // Failed: try to find the next concave vertex
            // 1) when find destination point failed
            // 2) when prev and i are too close that they are in the same line {origin[lineStartIndex], origin[lineEndIndex]}
            if(destinationPoint.equals(CCPointZero)
               || QIXGeometry::isPointInLineSegment(origin[lineStartIndex], origin[lineEndIndex], origin[i])
               || QIXGeometry::isPointSetCloseToLineSegments(pointSet, origin)) {
                CCLOG("***** Patition polygon failed: can't find suitable intersection point *****");
                continue;
            }
            
            // partition result
            std::vector< QIXPolygon > dividedParts;
            // partition by line segments
            bool flag = QIXGeometry::partitionPolygonByLineSegments(origin, pointSet, dividedParts, true);
            if (flag) {
                // partition succeeded
                if (debug) {
                    QIXGeometryInstance.drawNormalPartitionSegments(pointSet);
                }
                // next recursive run
                bool retValue = QIXGeometry::partitionPolygonIntoConvexParts(dividedParts[0], result, debug);
                if (retValue) {
                    retValue = QIXGeometry::partitionPolygonIntoConvexParts(dividedParts[1], result, debug);
                }
                return retValue;
            } else {
                // partition failed, try next
                CCLOG("***** Patition polygon failed: partitionPolygonByLineSegments *****");
                continue;
            }
        }
    }
    
    if (isConvex) {
        // it is an convex polygon
        result.push_back(origin);
        return true;
    } else {
        // cancave vertex exists, but partition failed
        if (debug) {
            for (int i=0; i<concavePoints.size(); i++) {
                QIXGeometryInstance.drawErrorPoint(concavePoints[i]);
            }
        }
        return false;
    }
    
}

void QIXGeometry::logPolygonPointers(const QIXPolygon &origin) {
    for (int i=0; i<origin.size(); i++) {
        CCLOG("{ %f, %f }", origin[i].x, origin[i].y);
    }
}

void QIXGeometry::simplifyLineSegments(QIXPointSet& origin) {
    for (QIXPointSet::iterator itVec = origin.begin(); itVec+2 != origin.end();) {
        // simplify if points are in line
        if (QIXGeometry::isPointInLineSegment(*itVec, *(itVec+1), *(itVec+2))) {
            origin.erase(itVec+1);
        } else {
            ++itVec;
        }
    }
}

bool QIXGeometry::isPointCloseToLineSegments(const QIXPolygon& line, CCPoint point, bool isPolygon) {
    int lineCount = isPolygon ? line.size() : line.size()-1;
    for (int i=0; i<lineCount; i++) {
        CCPoint pointStart = line[i];
        int index_next = (i+1) % line.size();
        CCPoint pointEnd = line[index_next];
        if (QIXGeometry::isPointCloseToLineSegment(pointStart, pointEnd, point)) {
            return true;
        }
    }
    return false;
}

bool QIXGeometry::isPathPointCloseToPolygon(const QIXPolygon& polygon, const QIXPointSet& pointSet) {
    for (int i=1; i<pointSet.size()-1; i++) {
        if (QIXGeometry::isPointCloseToLineSegments(polygon, pointSet[i], true)) {
            return true;
        }
    }
    return false;
}

bool QIXGeometry::isPointSetCloseToLineSegments(const QIXPolygon& line, const QIXPointSet& pointSet) {
    for (int i=0; i<pointSet.size(); i++) {
        if (!QIXGeometry::isTheSamePoint(line[0], pointSet[i])
            && !QIXGeometry::isTheSamePoint(line[line.size()-1], pointSet[i])
            && QIXGeometry::isPointCloseToLineSegments(line, pointSet[i], false)) {
            return true;
        }
    }
    return false;
}