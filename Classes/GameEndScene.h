//
//  GameEndScene.h
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#ifndef __QIXGame__GameEndScene__
#define __QIXGame__GameEndScene__

#include "cocos2d.h"
#include "QIXHelper.h"
#include "GameEndStatusLayer.h"

/**
 @brief Defines a game end scene
 @class GameEndScene
 
 Defines a game end scene
 It displays the game score
 */
class GameEndScene : public cocos2d::CCScene {
public:
    //! create an instance
    CREATE_FUNC(GameEndScene);
    
    //! deal with events
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    GameEndScene();
    //! destruct
    virtual ~GameEndScene();
    
    //! add background
    void addBackground();
    
private:
    //! background
    cocos2d::CCSprite* m_pBackground;
    //! statue layer
    GameEndStatusLayer* m_pGameEndStatusLayer;
};

#endif /* defined(__QIXGame__GameEndScene__) */
