//
//  GameStatus.h
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#ifndef __QIXGame__GameStatus__
#define __QIXGame__GameStatus__

#include "cocos2d.h"
#include "QIXMacros.h"


/**
 @brief Defines a game status
 @class GameStatus
 
 Defines a game status
 */
class GameStatus {
public:
    //! reset status
    void reset();
    //! go to next game level
    void goToNextLevel();
    
    //! increase launch count
    void increaseLaunchCount() {
        ++m_iLaunchCount;
    }
    
    //! increase play time
    void increasePlayTime() {
        ++m_iPlayTime;
    }
    
    //! map area clear ratio (*100)
    CC_SYNTHESIZE(int, m_iAreaClearRatio, AreaClearRatio);
    //! how many killed enemies
    CC_SYNTHESIZE(int, m_iKilledEnemyCount, KilledEnemyCount);
    //! how many enemies
    CC_SYNTHESIZE(int, m_iTotalEnemyCount, TotalEnemyCount);
    //! how many times the player is killed
    CC_SYNTHESIZE(int, m_iPlayerMissCount, PlayerMissCount);
    //! play time of the current scene
    CC_SYNTHESIZE(int, m_iPlayTime, PlayTime);
    //! whether the current scene passed or not
    CC_SYNTHESIZE(bool, m_bPass, Pass);
    //! the game level
    CC_SYNTHESIZE(int, m_iLevel, Level);
    //! the launch count
    CC_SYNTHESIZE(int, m_iLaunchCount, LaunchCount);
    
    //! construct
    GameStatus();
    //! destruct
    virtual ~GameStatus();
};

/**
 for global access
 */
extern GameStatus GameStatusInstance;

#endif /* defined(__QIXGame__GameStatus__) */
