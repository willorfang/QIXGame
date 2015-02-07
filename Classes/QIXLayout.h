//
//  QIXLayout.h
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#ifndef __QIXGame__QIXLayout__
#define __QIXGame__QIXLayout__

#include "cocos2d.h"
#include "QIXMacros.h"

/**
 @brief Defines the layout of game scene
 @class QIXLayout
 
 Defines the layout of game scene
 */
class QIXLayout : public cocos2d::CCObject
{
public:
    //! get screen rect
    static cocos2d::CCRect getScreenRect();
    //! get world boundary rect
    static cocos2d::CCRect getWorldBoundrayRect();
    //! get background rect
    static cocos2d::CCRect getBackgroundRect();
    //! get player area rect
    static cocos2d::CCRect getPlayerAreaRect();
    //! get map area rect
    static cocos2d::CCRect getMapAreaRect();
    //! get top panel rect
    static cocos2d::CCRect getTopPanelRect();
    //! get bottom panel rect
    static cocos2d::CCRect getBottomPanelRect();
    //! get the center of the screen
    static cocos2d::CCPoint getScreenCenter();
    //! get absolute position from relative
    static void changeToAbsolutePositionFromRelative(cocos2d::CCPoint& origin,
                                                     const cocos2d::CCRect& area);
    static void getAbsolutePositionWithScreenRect(cocos2d::CCPoint& origin);
};

#endif /* defined(__QIXGame__QIXLayout__) */
