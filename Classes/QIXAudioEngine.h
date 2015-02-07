//
//  QIXAudioEngine.h
//  QIXGame
//
//  Created by Wei Fang on 4/23/14.
//
//

#ifndef __QIXGame__QIXAudioEngine__
#define __QIXGame__QIXAudioEngine__

#include "cocos2d.h"
#include "QIXMacros.h"

/**
 @brief A wrapper of the real audio engine
 @class QIXAudioEngine
 
 A wrapper of the real audio engine
 */

class QIXAudioEngine{
public:
    //! background music function
    void playBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float volume);
    
    //! define all effects
    void playGamePass();
    void playGameFail();
    void playPlayerKilled();
    void playPlayerClear();
    void playFrozen();
    void playPlayerLaunch();
    void playTouchBan();
    
    //! effect function
    void playEffect(const char * name);
    void pauseAllEffects();
    void resumeALLEffects();
    void stopALLEffects();
    void setEffectsVolume(float volume);
    
    //! initialize
    void initialize();
    //! purge cache data
    void purge();
    
    //! preload sound resources
    static void preloadBackgroundMusic(const char* filename);
    static void preloadEffect(const char* filename);

    //! construct
    QIXAudioEngine();
    //! destruct
    virtual ~QIXAudioEngine();
};

/**
 for global access
 */
extern QIXAudioEngine QIXAudioEngineInstance;

#endif /* defined(__QIXGame__QIXAudioEngine__) */
