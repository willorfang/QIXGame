//
//  GameLoadScene.h
//  QIXGame
//
//  Created by Wei Fang on 5/16/14.
//
//

#ifndef __QIXGame__GameLoadScene__
#define __QIXGame__GameLoadScene__

#include "cocos2d.h"
#include "QIXHelper.h"

/**
 @brief Defines a game load scene
 @class GameLoadScene
 
 Defines a game load scene
 Preload all resources HERE
 */
class GameLoadScene : public cocos2d::CCScene{
public:
    //! create an instance
    CREATE_FUNC(GameLoadScene);
    
    //! deal with events
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    GameLoadScene();
    //! destruct
    virtual ~GameLoadScene();
    
    //! add background
    void addBackground();
    //! add progress timer
    void addProgressTimer();
    void addProgressTimerBackgournd();
    //! add title label
    void addTitleLabel();
    //! add loading label
    void addLoadingLabel();
    
    //! load resources
    void loadResources();
    //! callback for loading image
    void loadAllImagesCallbackFunc(CCObject* obj);
    
    
    //! update progress
    virtual void update(float dt);
    
private:
    //! background
    cocos2d::CCSprite* m_pBackground;
    //! game tile label
    cocos2d::CCLabelBMFont* m_pGameTitleLabel;
    //! loading label
    cocos2d::CCLabelBMFont* m_pLoadingLabel;
    
    //! load progress related
    cocos2d::CCSprite* m_pLoadProgressTimerBackground;
    cocos2d::CCProgressTimer* m_pLoadProgressTimer;
    
    /**
     create worker thread to load resources
     */
    //! communicate structure between main thread and worker thread
    struct ResourceLoadIndicator{
        ResourceLoadIndicator() : loadProgress(0), loadTotal(100) {
        }
        int loadProgress;
        int loadTotal;
    };
    ResourceLoadIndicator m_sResourceLoadIndicator;
};

#endif /* defined(__QIXGame__GameLoadScene__) */
