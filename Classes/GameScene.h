//
//  GameScene.h
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#ifndef __QIXGame__GameScene__
#define __QIXGame__GameScene__

#include "cocos2d.h"
#include "QIXHelper.h"
#include "Box2DWorld.h"
#include "PlayerLayer.h"
#include "MapLayer.h"
#include "PanelLayer.h"
#include "IntroLayer.h"
#include "GameEndScene.h"

/**
 @brief Defines a game scene
 @class GameScene
 
 Defines a game scene.
 It is the main game-play scene
 */
class GameScene : public cocos2d::CCScene
{
public:
    //! create an instance
    CREATE_FUNC(GameScene);

    //! deal with collision
    virtual void onCollisionEvent(B2Node* nodeA, B2Node* nodeB);
    
    //! get layer references
    PanelLayer* getPanelLayer() {
        CCAssert(NULL != m_pPanelLayer, "");
        return m_pPanelLayer;
    }
    MapLayer* getMapLayer() {
        CCAssert(NULL != m_pMapLayer, "");
        return m_pMapLayer;
    }
    PlayerLayer* getPlayerLayer() {
        CCAssert(NULL != m_pPlayerLayer, "");
        return m_pPlayerLayer;
    }
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    GameScene();
    //! destruct
    virtual ~GameScene();
    
    //! pause the scene except the intro layer
    void pauseTheSceneExceptIntroPanel();
    
    //! deal with events
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    //! notification message
    void receiveNotificationMessage(cocos2d::CCObject* obj);
    void registerNotificationMessage();
    void unregisterNotificationMessage();
    
    //! get MapArea instance
    MapArea* getMapArea();
    
    //! update game status
    virtual void update(float dt);
    //! update play time
    void updatePlayTime();
    
    //! initialize box2d world
    void initializeBox2DWorld();
    
    //! initialize background
    void initializeBackground();

private:
    //! player layer
    PlayerLayer* m_pPlayerLayer;
    //! map layer
    MapLayer* m_pMapLayer;
    //! panel layer
    PanelLayer* m_pPanelLayer;
    //! intro layer
    IntroLayer* m_pIntroLayer;
    
    //! background sprite
    cocos2d::CCSprite* m_pEdgeBackground;
    cocos2d::CCSprite* m_pPlayerBackground;

    //! whether the current game scene finished or not
    bool m_bSceneFinished;
};

#endif /* defined(__QIXGame__GameScene__) */
