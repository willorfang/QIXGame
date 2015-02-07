//
//  QIXGeometry.h
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#ifndef __QIXGame__QIXGeometry__
#define __QIXGame__QIXGeometry__

#include "cocos2d.h"
#include "QIXMacros.h"

typedef std::vector<cocos2d::CCPoint> QIXPointSet;
typedef std::vector<cocos2d::CCPoint> QIXPolygon;

/**
 @brief Defines geometric algorithms
 @class QIXGeometry
 
 Defines geometric algorithms
 */
class QIXGeometry
{
public:
    /*************************************************************************
     debug draw related helper
     *************************************************************************/
    
    /*************************************************************************
     geometry related algorithms
     *************************************************************************/
    
    //! get the intersection point between the line segment and the ray if any
    /**
     @param linePointA a vertex of the line segment
     @param linePointB another vertex of the line segment
     @param rayPointA the vertex of the ray
     @param rayPointB a point on the ray
     @param intersection the intersection point
     @return whether the two intersected
     */
    static bool getIntersectionOfLineSegmentAndRay(cocos2d::CCPoint linePointA,
                                                   cocos2d::CCPoint linePointB,
                                                   cocos2d::CCPoint rayPointA,
                                                   cocos2d::CCPoint rayPointB,
                                                   cocos2d::CCPoint& intersection);
    
    //! get the first intersection point between the polygon and the ray
    /**
     CAUTION: the polygon verteces should be in the clockwise direction
     
     DETAIL:
     1) draw a ray from origin[index] to origin[index+1].
     2) get the first intersection point if any
     
     @param origin polygon verteces in the clockwise direction
     @param index the start point of the ray
     @return the nearest intersection point
     */
    static cocos2d::CCPoint getFirstIntersectionPointWithPolygon(const QIXPolygon& origin,
                                                                 int index,
                                                                 int& lineStartIndex,
                                                                 int& lineEndIndex);
    
    //! whether two point are the same or not
    static bool isTheSamePoint(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB);
    
    //! whether a point is on a line segment
    /**
     @param linePointA a vertex of the line segment
     @param linePointB another vertex of the line segment
     @param point the point
     @return whether the point is on the line segment
     */
    static bool isPointInLineSegment(cocos2d::CCPoint lineSegmentPointA,
                                     cocos2d::CCPoint lineSegmentPointB,
                                     cocos2d::CCPoint point);
    
    static bool isPointCloseToLineSegment(cocos2d::CCPoint lineSegmentPointA,
                                     cocos2d::CCPoint lineSegmentPointB,
                                     cocos2d::CCPoint point);
protected:
    static bool judgePointAndLineSegment(cocos2d::CCPoint lineSegmentPointA,
                                        cocos2d::CCPoint lineSegmentPointB,
                                        cocos2d::CCPoint point,
                                        float distance);
public:
    //! get the size of a polygon area
    /**
     @param origin polygon verteces in the clockwise direction
     @return the size of the polygon
     */
    static float getPolygonAreaSize(const QIXPolygon& origin);
    
    //! whether a point is in a polygon or not
    /**
     CAUTION: vertices on some side of the polygon is considered as OUT
     
     @param origin polygon verteces in the clockwise direction
     @param point the point
     @return whether the point is in the polygon or not
     */
    static bool polygonContainsPoint(const QIXPolygon& origin, cocos2d::CCPoint point);
    
    //! patition concave polygon into convex polygons
    /**
     CAUTION: the polygon verteces should be in the clockwise direction
     
     @param origin polygon verteces in the clockwise direction
     @param result the created convex polygon array
     @param whether use debug draw or not
     @return partition succeed or not
     */
    static bool partitionPolygonIntoConvexParts(const QIXPolygon& origin,
                                                std::vector< QIXPolygon >& result);
protected:
    static bool partitionPolygonIntoConvexParts(const QIXPolygon& origin,
                                                std::vector< QIXPolygon >& result,
                                                bool debug);
public:
    static bool partitionPolygonIntoConvexPartsWithDebugDraw(
                                                const QIXPolygon& origin,
                                                std::vector< QIXPolygon >& result);
    
    //! divide polygon into two pieces by a line
    /**
     CAUTION:
     1) the polygon verteces should be in the clockwise direction
     2) the first and last point in lineSegments should on polygon‘s sides
     
     @param origin polygon verteces in the clockwise direction
     @param lineSegments partition line
     @param result the created convex polygon array
     @param callByInternal whether call
     @return whether divided successfully or not
     */
protected:
    static bool partitionPolygonByLineSegments(const QIXPolygon& origin,
                                               const QIXPointSet& lineSegments,
                                               std::vector< QIXPolygon >& result,
                                               bool callByInternal);
public:
    static bool partitionPolygonByLineSegments(const QIXPolygon& origin,
                                               const QIXPointSet& lineSegments,
                                               std::vector< QIXPolygon >& result);
    
    /**
     @brief whether it is intersected with itself
     @param origin lineSegments' verteces in the clockwise direction
     @return whether it is intersected between line segments or not
     */
    static bool isLineSegmentsIntersected(const QIXPointSet& origin);
    
    //! eg. if origin[i], origin[i+1], origin[i+2] are in one line, delete origin[i+1]
    /**
     @param origin line segments in the clockwise direction
     */
    static void simplifyLineSegments(QIXPointSet& origin);
    
    //! whether the point is on some edge of the line segments
    static bool isPointCloseToLineSegments(const QIXPolygon& line, cocos2d::CCPoint point, bool isPolygon);
    
    //! whether point sets are on some edge of the polygon (except for the first & last point of the pointSet)
    static bool isPathPointCloseToPolygon(const QIXPolygon& polygon, const QIXPointSet& pointSet);
    //! whether point sets are on some edge of line segments (except for the first & last point of the line)
    static bool isPointSetCloseToLineSegments(const QIXPolygon& line, const QIXPointSet& pointSet);
    
    //! log polygon verticles
    static void logPolygonPointers(const QIXPolygon& origin);
    
    //! construct
    QIXGeometry();
    
    //! destruct
    virtual ~QIXGeometry();
    
    //! parent node for all draw nodes
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pParentDrawNode, ParentDrawNode);
    
private:
    //! get the determinant
    /**
     | Ax Ay 1 |
     | Bx By 1 |
     | Cx Cy 1 |
     */
    static float getDeterminant(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, cocos2d::CCPoint pointC);
    
    //! debug draw a polygon
    void drawEdgePolygon(const QIXPolygon& origin);
    void drawErrorPolygon(const QIXPolygon& origin);
    void drawDebugPolygon(const QIXPolygon& origin, cocos2d::ccColor4F color, int zValue);
    //! debug draw a vertex
    void drawErrorPoint(cocos2d::CCPoint point);
    void drawNormalPoint(cocos2d::CCPoint point);
    void drawDebugPoint(cocos2d::CCPoint point, cocos2d::ccColor4F color, int zValue);
    //! debug draw a segment
    void drawNormalPartitionSegments(const QIXPointSet& origin);
    void drawErrorPartitionSegments(const QIXPointSet& origin);
    void drawDebugSegment(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB, cocos2d::ccColor4F color, int zValue);
    
    //! clear debug draw
    void clearDebugDraw();
};

#endif /* defined(__QIXGame__QIXGeometry__) */
