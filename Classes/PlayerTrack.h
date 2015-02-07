//
//  PlayerTrack.h
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#ifndef __QIXGame__PlayerTrack__
#define __QIXGame__PlayerTrack__

#include "cocos2d.h"
#include "QIXMacros.h"
#include "QIXHelper.h"

/**
 @brief Defines the player track
 @class PlayerTrack
 
 Defines the player track
 */
class PlayerTrack : public cocos2d::CCNode {
public:
    //! create an instance
    CREATE_FUNC(PlayerTrack);
    
    //! add track point
    void addPlayerTrackPoint(cocos2d::CCPoint point);
    //! clear track points
    void clearAllPlayerTrackPoints();
    
    //! should draw player's track or not from now on
    void beginTrack() {
        m_bWillDrawPlayerTrack = true;
    }
    void endTrack() {
        m_bWillDrawPlayerTrack = false;
        this->clearAllPlayerTrackPoints();
    }
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    PlayerTrack();
    //! destruct
    virtual ~PlayerTrack();
    
    //! draw player's track
    virtual void draw();
    
private:
    //! player's track points
    cocos2d::CCPointArray* m_pPlayerTrackPoints;
    //! the draw state: draw OR DON'T draw
    bool m_bWillDrawPlayerTrack;
};

#endif /* defined(__QIXGame__PlayerTrack__) */
