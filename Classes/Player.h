//
//  Player.h
//  QIXGame
//
//  Created by Wei Fang on 4/2/14.
//
//

#ifndef __QIXGame__Player__
#define __QIXGame__Player__

#include "B2Sprite.h"
#include "PlayerPath.h"
#include "ShowBox.h"

class PlayerEffect;
class PlayerTrack;

/**
 @brief Defines the player
 @class Player
 
 Defines the player
 */
class Player : public B2Sprite {
public:
    //! create the singleton
    SINGLETON_FUNC(Player);
    
    //! callback when it is killed
    virtual void beKilled();
    //! callback when it is attracted by Black Hole
    virtual void beAttracted(const cocos2d::CCPoint& point);
    //! whether it can be attracted or not
    virtual bool canBeAttracted();
    
    //! callback when conllided with some object
    virtual void onCollisionWithAnotherObject(B2Node* node);
    //! whether the object can response to collision or not
    virtual bool canResponseToCollision() {
        return !this->isSafe();
    }
    
    //! whether the player is in safe state or not
    bool isSafe() {
        return this->getSensor();
    }
    
    //! judge functions
    bool isMissReachedMax() {
        return m_iMissCount >= QIXResourceInstance.m_oLevelConfig.PassCondition.MissCountMax;
    }
    
    //! increase clear count
    void increaseClearCount(int clearedRatio);
    
    //! wrapper functions
    void runStepEffect();
    
    //! how many times the player is missed
    CC_SYNTHESIZE(int, m_iMissCount, MissCount);
    
    //! the previous position when the player and the collide area didn't collide
    CC_SYNTHESIZE(cocos2d::CCPoint, m_vPrevPosition, PrevPosition);
    
    //! whether it is moving in the previous frame or not
    CC_SYNTHESIZE(bool, m_bIsPrevMoving, IsPrevMoving);
    
    //! the player path
    CC_SYNTHESIZE(PlayerPath*, m_pPlayerPath, PlayerPath);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    Player();
    //! destruct
    virtual ~Player();
    
    //! get player effect reference
    PlayerEffect* getPlayerEffect();
    //! get player track reference
    PlayerTrack* getPlayerTrack();
    
    //! update player's state
    virtual void update(float dt);
    
    //! reset to default state
    void reset();
    //! miss call func
    void missCallFunc();
    
    //! set player state sprite
    void setInitSprite();
    void setSafeSprite();
    void setActionSprite();
    void setSafeSpriteWithAnimation();
    void setActionSpriteWithAnimation();
    
    //! initialize step show box
    void initialzieStepShowBox();
    //! update step show box label
    void updateStepShowBoxLabel(const char* str);
    void updateStepShowBoxLabel(int stepNum);
    
    //! initialize state sprites for safe & action
    void initializeStateSprites();
    
private:
    //! the singleton
    static Player* m_pInstance;
    
    //! the player sprite
    cocos2d::CCSprite* m_pPlayerSafeSprite;
    cocos2d::CCSprite* m_pPlayerActionSprite;
    
    //! whether the player is safety
    bool m_bIsSafety;
    //! whether the player is ready to deal with event
    bool m_bIsActive;
    
    //! step show box
    ShowBox* m_pStepShowBox;
};

#endif /* defined(__QIXGame__Player__) */
