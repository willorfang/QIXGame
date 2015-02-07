//
//  PlayerPath.h
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#ifndef __QIXGame__PlayerPath__
#define __QIXGame__PlayerPath__

#include "cocos2d.h"
#include "QIXMacros.h"
#include "QIXHelper.h"

/**
 @brief Defines the player path
 @class PlayerPath
 
 Defines the player path
 */
class PlayerPath : public cocos2d::CCNode {
public:
    //! create an instance
    CREATE_FUNC(PlayerPath);
    
    //! judge functions
    bool isPathReached() {
        return QIXResourceInstance.m_oGlobalConfig.PlayerClearPathLength + 1 == m_vPathPoints.size();
    }
    bool isPathDidNotReach() {
        return QIXResourceInstance.m_oGlobalConfig.PlayerClearPathLength + 1 > m_vPathPoints.size();
    }
    bool isPathExceeded() {
        return QIXResourceInstance.m_oGlobalConfig.PlayerClearPathLength + 1 < m_vPathPoints.size();
    }
    bool isPathWillReach() {
        return QIXResourceInstance.m_oGlobalConfig.PlayerClearPathLength == m_vPathPoints.size();
    }
    
    //! deal with player path points
    void clearPathPoints() {
        m_vPathPoints.clear();
//        CCLOG("Clear player path points");
    }
    int getPathPointsSize() {
        return m_vPathPoints.size();
    }
    int getPathSize() {
        return m_vPathPoints.size() - 1;
    }
    bool isPathPointsEmpty () {
        return m_vPathPoints.size() == 0 ? true : false;
    }
    //! get index of a path point
    int getIndexOfPathPoint(cocos2d::CCPoint point) {
        int index = -1;
        for (int i=0; i<m_vPathPoints.size(); ++i) {
            if (m_vPathPoints[i].equals(point)) {
                index = i;
                break;
            }
        }
        return index;
    }
    
    //! when player moves into map area
    void addFirstPathPoint(cocos2d::CCPoint point);
    //! when touch, collision happens
    void addMiddlePathPoint(cocos2d::CCPoint point);
    //! when player moves out of map area
    void addLastPathPoint(cocos2d::CCPoint point);
    
    //! player movement path
    CC_SYNTHESIZE_READONLY(QIXPointSet, m_vPathPoints, PathPoints);
    //! whether the last path point is added or not
    CC_SYNTHESIZE(bool, m_bIsLastPathPointAdded, LastPathPointAdded);

protected:
    //! initialize
    virtual bool init();
    //! construct
    PlayerPath();
    //! destruct
    virtual ~PlayerPath();
    
    //! add a point into path
    void addPathPoint(cocos2d::CCPoint point) {
        m_vPathPoints.push_back(point);
//        CCLOG("Add a player path point: [%f, %f]", point.x, point.y);
    }
};

#endif /* defined(__QIXGame__PlayerPath__) */
