//
//  GameStatus.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#include "GameStatus.h"
#include "QIXHelper.h"
#include "B2Node.h"

USING_NS_CC;

/* the instance */
GameStatus GameStatusInstance;

void GameStatus::reset() {
    m_iAreaClearRatio = 0;
    m_iKilledEnemyCount = 0;
    m_iTotalEnemyCount = 0;
    m_iPlayerMissCount = 0;
    m_iPlayTime = 0;
    m_bPass = false;
    m_iLaunchCount = 0;
}

GameStatus::GameStatus() {
    CCLOG("GameStatus instance is created");
    m_iLevel = 1;
    this->reset();
}

GameStatus::~GameStatus() {
}

void GameStatus::goToNextLevel() {
    m_iLevel %= QIXResourceInstance.m_oGlobalConfig.GameLevelMax;
    ++m_iLevel;
}