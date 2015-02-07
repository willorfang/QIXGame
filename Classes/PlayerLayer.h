//
//  PlayerLayer.h
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#ifndef __QIXGame__PlayerLayer__
#define __QIXGame__PlayerLayer__

#include "cocos2d.h"
#include "QIXMacros.h"
#include "Player.h"
#include "PlayerEffect.h"
#include "PlayerTrack.h"

/**
 @brief Defines the player layer
 @class PlayerLayer
 
 Defines the player layer
 1) deal with touch event
 2) draw player's track
 3) update player's path points
 */
class PlayerLayer : public cocos2d::CCLayer
{
public:
    //! create an instance
    CREATE_FUNC(PlayerLayer);
    
    //! deal with touch events
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //! set touch event enabled / disabled
    virtual void registerWithTouchDispatcher();
    
    //! reset the player
    void resetPlayer();
    //! attract the player
    void attractPlayer(cocos2d::CCPoint point);
    
    // the player
    CC_SYNTHESIZE(Player*, m_pPlayer, Player);
    //! manage player effects
    CC_SYNTHESIZE_READONLY(PlayerEffect*, m_pPlayerEffect, PlayerEffect);
    //! manage player track
    CC_SYNTHESIZE_READONLY(PlayerTrack*, m_pPlayerTrack, PlayerTrack);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    PlayerLayer();
    //! destruct
    virtual ~PlayerLayer();
    
    //! change the position from UI coordinate to GL coordinate
    cocos2d::CCPoint locationFromTouch(cocos2d::CCTouch *touch);
    
    //! initialize arrow
    void addArrow();
    //! initialize ban
    void addBan();
    //! show ban
    void showBan(cocos2d::CCPoint point);
    //! callback for showBan
    void showBanCallback();

private:
    //! the arrow sprite
    cocos2d::CCSprite* m_pArrowSprite;
    //! the ban sprite
    cocos2d::CCSprite* m_pBanSprite;
    //! the points when touch begins and ends
    std::pair<cocos2d::CCPoint, cocos2d::CCPoint> m_vTouchPoints;
    //! whether the player is launching or not
    bool m_bIsLaunching;
    //! whether the player launch is canceled or not
    bool m_bIsLaunchCanceled;
};

#endif /* defined(__QIXGame__PlayerLayer__) */
