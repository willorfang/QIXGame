//
//  QIXEffect.h
//  QIXGame
//
//  Created by Wei Fang on 5/8/14.
//
//

#ifndef __QIXGame__QIXEffect__
#define __QIXGame__QIXEffect__

#include "cocos2d.h"
#include "QIXMacros.h"

typedef enum {
    kDirectionFromTop = 0,
    kDirectionFromBottom,
    kDirectionFromLeft,
    kDirectionFromRight,
    kDirectionToTop,
    kDirectionToBottom,
    kDirectionToLeft,
    kDirectionToRight
} kDirectionType;

/**
 @brief Defines a amount of effects
 @class QIXEffect
 
 Defines a amount of effects
 */
class QIXEffect
{
public:
    //! scale out and back effect
    static cocos2d::CCActionInterval* createScaleOutAndBack(
                                                int repeat = 1,
                                                cocos2d::CCCallFuncND* callbackFunc = NULL);
    
    //! change for startValue to endValue
    static cocos2d::CCActionInterval* createScaleOutAndBackWithShow(
                                                int startValue,
                                                int endValue,
                                                cocos2d::CCCallFuncND* callbackFunc = NULL);
    
    
    //! splash show effect
    static cocos2d::CCActionInterval* createSplashShow(
                                                cocos2d::CCNode* node,
                                                cocos2d::CCCallFuncND* callbackFunc = NULL);
    
    //! ease out move in / out effect
    static cocos2d::CCActionInterval* createMoveInOutWithEase(
                                                cocos2d::CCNode* node,
                                                kDirectionType type,
                                                cocos2d::CCCallFuncND* callbackFunc = NULL);
    
    //! button emphasize effect
    static cocos2d::CCActionInterval* createEmphasizeButton(float scaleOrigin, int repeat = -1);
    
    //! bounce effect
    static cocos2d::CCActionInterval* createBounceUpAndBack(int repeat = -1);
    
    //! creep effect
    static cocos2d::CCActionInterval* createCreep(int repeat = -1);
};

#endif /* defined(__QIXGame__QIXEffect__) */
