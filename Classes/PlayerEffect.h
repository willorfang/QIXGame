//
//  PlayerEffect.h
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#ifndef __QIXGame__PlayerEffect__
#define __QIXGame__PlayerEffect__

#include "cocos2d.h"
#include "QIXMacros.h"

/**
 @brief Defines the player effect
 @class PlayerEffect
 
 Defines the player effect
 */
class PlayerEffect : public cocos2d::CCNode
{
public:
    //! create an instance
    CREATE_FUNC(PlayerEffect);
    
    //! run effect
    void runMissEffect(cocos2d::CCPoint point);
    void runClearEffect(cocos2d::CCPoint point, int ratio);
    void runSkipEffect(cocos2d::CCPoint point);
    void runStepEffect(cocos2d::CCPoint point);
    void runChangeStateEffect(cocos2d::CCNode* node);
    
protected:
    //! run default font effect
    void runDefaultFontEffect(std::string content, cocos2d::CCPoint point);
    //! default Callback func
    void defaultCallFunc(cocos2d::CCNode *node, void* data);
    
    //! initialize
    virtual bool init();
    //! construct
    PlayerEffect();
    //! destruct
    virtual ~PlayerEffect();
};

#endif /* defined(__QIXGame__PlayerEffect__) */
