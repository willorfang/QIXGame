//
//  IntroLayer.h
//  QIXGame
//
//  Created by Wei Fang on 5/27/14.
//
//

#ifndef __QIXGame__IntroLayer__
#define __QIXGame__IntroLayer__

#include "cocos2d.h"
#include "QIXMacros.h"

#include <stack>

/**
 @brief Defines the introduction layer
 @class IntroLayer
 
 Defines the introduction layer
 */
class IntroLayer : public cocos2d::CCLayer
{
public:
    //! create an instance
    CREATE_FUNC(IntroLayer);
    
    //! deal with touch events
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //! whether the intro panel is on show
    bool isOnShow() {
        CCAssert(NULL != m_pIntroPanel, "");
        return m_pIntroPanel->isVisible();
    }
    
    //! show level target introduction
    void showTargetIntro();
    //! show level result introduction
    void showResultIntro();
    void showResultIntroAfterSomeTime(float time);
protected:
    void showIntro(std::string title, std::string info);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    IntroLayer();
    //! destruct
    virtual ~IntroLayer();
    
    //! set touch event enabled / disabled
    virtual void registerWithTouchDispatcher();
    //! change the position from UI coordinate to GL coordinate
    cocos2d::CCPoint locationFromTouch(cocos2d::CCTouch *touch);
    
    //! initialize introduction panel
    void initializeIntroPanel();
    void introPanelCallFunc(cocos2d::CCNode* node, void* data);
    
private:
    //! the intro panel
    cocos2d::CCSprite* m_pIntroPanel;
};

#endif /* defined(__QIXGame__IntroLayer__) */
