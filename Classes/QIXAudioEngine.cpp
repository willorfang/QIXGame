//
//  QIXAudioEngine.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/23/14.
//
//

#include "QIXAudioEngine.h"
#include "SimpleAudioEngine.h"
#include "QIXHelper.h"

USING_NS_CC;
using namespace CocosDenshion;

/* the instance */
QIXAudioEngine QIXAudioEngineInstance;

static SimpleAudioEngine* s_pAudioEngine = NULL;

QIXAudioEngine::QIXAudioEngine() {
    CCLOG("QIXAudioEngine instance is created");
}

void QIXAudioEngine::initialize() {
    s_pAudioEngine = SimpleAudioEngine::sharedEngine();
}

QIXAudioEngine::~QIXAudioEngine() {
    this->purge();
}

void QIXAudioEngine::preloadBackgroundMusic(const char* filename) {
    CCAssert(NULL != filename, "");
    s_pAudioEngine->preloadBackgroundMusic(filename);
}

void QIXAudioEngine::preloadEffect(const char* filename) {
    CCAssert(NULL != filename, "");
    s_pAudioEngine->preloadEffect(filename);
}

void QIXAudioEngine::playBackgroundMusic() {
    const char* filename = QIXResourceInstance.m_oGlobalConfig.BackgroundMusic.c_str();
    s_pAudioEngine->playBackgroundMusic(filename, true);
}

void QIXAudioEngine::pauseBackgroundMusic() {
    s_pAudioEngine->pauseBackgroundMusic();
}

void QIXAudioEngine::resumeBackgroundMusic() {
    s_pAudioEngine->resumeBackgroundMusic();
}

void QIXAudioEngine::stopBackgroundMusic() {
    s_pAudioEngine->stopBackgroundMusic();
}

void QIXAudioEngine::setBackgroundMusicVolume(float volume) {
    s_pAudioEngine->setBackgroundMusicVolume(volume);
}

void QIXAudioEngine::playEffect(const char * name) {
    CCAssert(NULL != name, "");
    s_pAudioEngine->playEffect(name);
}

void QIXAudioEngine::pauseAllEffects() {
    s_pAudioEngine->pauseAllEffects();
}

void QIXAudioEngine::resumeALLEffects() {
    s_pAudioEngine->resumeAllEffects();
}

void QIXAudioEngine::stopALLEffects() {
    s_pAudioEngine->stopAllEffects();
}

void QIXAudioEngine::setEffectsVolume(float volume) {
    s_pAudioEngine->setEffectsVolume(volume);
}

void QIXAudioEngine::playGamePass() {
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.GamePassSound.c_str());
}

void QIXAudioEngine::playGameFail() {
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.GameFailSound.c_str());
}

void QIXAudioEngine::playPlayerKilled() {
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.PlayerKilledSound.c_str());
}

void QIXAudioEngine::playPlayerClear() {
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.AreaClearSound.c_str());
}

void QIXAudioEngine::playPlayerLaunch() {
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.PlayerLaunchSound.c_str());
}

void QIXAudioEngine::playTouchBan(){
    this->playEffect(QIXResourceInstance.m_oGlobalConfig.PlayerLaunchBanSound.c_str());
}

void QIXAudioEngine::purge() {
    // end the audio engine
    s_pAudioEngine->end();
}
