//
//  GameLoginScene.h
//  QIXGame
//
//  Created by Wei Fang on 5/19/14.
//
//

#ifndef __QIXGame__GameLoginScene__
#define __QIXGame__GameLoginScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "QIXHelper.h"

/**
 @brief Defines a game login scene
 @class GameLoginScene
 
 Defines a game login scene
 */
class GameLoginScene : public cocos2d::CCScene{
public:
    //! create an instance
    CREATE_FUNC(GameLoginScene);
    
    //! deal with events
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    GameLoginScene();
    //! destruct
    virtual ~GameLoginScene();
    
    //! add background
    void addBackground();
    //! add title label
    void addTitleLabel();
    //! add replay button
    void addPlayButton();
    //! callback for replay button
    void playButtonCallBack(cocos2d::CCObject *object,
                              cocos2d::extension::CCControlEvent controlEvent);
    
private:
    //! background
    cocos2d::CCSprite* m_pBackground;
    //! game tile label
    cocos2d::CCLabelBMFont* m_pGameTitleLabel;
    //! the button
    cocos2d::extension::CCControlButton* m_pButton;
};


#endif /* defined(__QIXGame__GameLoginScene__) */
