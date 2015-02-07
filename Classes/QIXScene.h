//
//  QIXScene.h
//  QIXGame
//
//  Created by Wei Fang on 5/23/14.
//
//

#ifndef __QIXGame__QIXScene__
#define __QIXGame__QIXScene__

#include "cocos2d.h"
#include "QIXMacros.h"

/**
 @brief Defines a amount of scene-change functions
 @class QIXScene
 
 Defines a amount of scene-change functions
 */
class QIXScene : public cocos2d::CCObject
{
public:
    static void runGameLoadScene();
    static void goToGameLoginScene();
    static void goToGameScene();
    static void goToGameEndScene();
    
private:
    template<typename GAMESCENE>
    static void goToNextScene();
};

#endif /* defined(__QIXGame__QIXScene__) */
