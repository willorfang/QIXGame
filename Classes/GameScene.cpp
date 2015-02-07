//
//  GameScene.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#include "GameScene.h"
#include "QIXHelper.h"
#include "QIXAudioEngine.h"

//! map_layer < player_layer < panel_layer
#define LAYOUT_ORDER_MAP_LAYER          1
#define LAYOUT_ORDER_PLAYER_LAYER       2
#define LAYOUT_ORDER_PANEL_LAYER        3
#define LAYOUT_ORDER_INTRO_LAYER        4

USING_NS_CC;

GameScene::GameScene() {
    CCLOG("====================Game Scene====================");
}

GameScene::~GameScene() {

}

void GameScene::onEnter() {
    CCScene::onEnter();
    
    // pause the scene
    this->pauseTheSceneExceptIntroPanel();
}

void GameScene::pauseTheSceneExceptIntroPanel() {
    // pause the scene except for intro player
    QIXCocos2dHelper::pauseAllOfRoot(this);
    QIXCocos2dHelper::resumeAllOfRoot(m_pIntroLayer);
}

void GameScene::onEnterTransitionDidFinish() {
    CCScene::onEnterTransitionDidFinish();
    
    // play background music
    QIXAudioEngineInstance.playBackgroundMusic();
    
    // register CCNotificationCenter message
    this->registerNotificationMessage();
}

void GameScene::onExit() {
    CCScene::onExit();
    
    // stop background music and all sound effects
    QIXAudioEngineInstance.stopBackgroundMusic();
    QIXAudioEngineInstance.stopALLEffects();
    
    // unregister CCNotificationCenter message
    this->unregisterNotificationMessage();
}


bool GameScene::init()
{
    if(CCScene::init()){
        // default setting
        m_bSceneFinished = false;
        
        // initialize the box2d world
        this->initializeBox2DWorld();
        
        // load resource for level
        int level = GameStatusInstance.getLevel();
        QIXResourceInstance.parseInfoForNewLevel(level);
        CCLOG("Game Level: %d", level);
        
        // initialize the background
        this->initializeBackground();

        // add map
        m_pMapLayer = MapLayer::create();
        this->addChild(m_pMapLayer, LAYOUT_ORDER_MAP_LAYER);
        
        // add player
        m_pPlayerLayer = PlayerLayer::create();
        this->addChild(m_pPlayerLayer, LAYOUT_ORDER_PLAYER_LAYER);
        
        // add panel
        m_pPanelLayer = PanelLayer::create();
        this->addChild(m_pPanelLayer, LAYOUT_ORDER_PANEL_LAYER);
        
        // add intro
        m_pIntroLayer = IntroLayer::create();
        this->addChild(m_pIntroLayer, LAYOUT_ORDER_INTRO_LAYER);
        // show target intro
        m_pIntroLayer->showTargetIntro();
        
        // update play time
        this->schedule(schedule_selector(GameScene::updatePlayTime), 1.0f, INT_MAX, 1.0f);
        
        // register update()
        this->scheduleUpdate();
        
        return true;
    }
    return false;
}

void GameScene::initializeBox2DWorld() {
    Box2DWorld* world = Box2DWorld::GetTheBox2DWorld();
    // set world boundary
    CCRect worldEdge = QIXLayout::getWorldBoundrayRect();
    world->createBoundaryFromRect(worldEdge);
    // set contact listener
    QIXB2ContactListener* listener = QIXB2ContactListener::create();
    listener->setDelegate(this);
    world->setContactListenerDelegate(listener);
    this->addChild(world);
}

void GameScene::onCollisionEvent(B2Node* nodeA, B2Node* nodeB) {
    nodeA->onCollisionWithAnotherObject(nodeB);
    nodeB->onCollisionWithAnotherObject(nodeA);
}

void GameScene::updatePlayTime() {
    GameStatusInstance.increasePlayTime();
    int display = QIXResourceInstance.m_oLevelConfig.PassCondition.PlayTimeMax - GameStatusInstance.getPlayTime();
    m_pPanelLayer->setPlayTime(display);
}

void GameScene::update(float dt) {
    Player* player = Player::GetThePlayer();
    CCAssert(NULL != player, "");
    
    GameStatusInstance.setAreaClearRatio(m_pMapLayer->getAreaClearRatio());
    GameStatusInstance.setKilledEnemyCount(m_pMapLayer->getKilledEnemyCount());
    GameStatusInstance.setTotalEnemyCount(m_pMapLayer->getTotalEnemyCount());
    GameStatusInstance.setPlayerMissCount(player->getMissCount());
    
    // Fail cases:
    // 1) player's miss OR Clear has reached the max value
    // 2) stage is blocked
    int PlayTimeMax = QIXResourceInstance.m_oLevelConfig.PassCondition.PlayTimeMax;
    int LaunchCountMax = QIXResourceInstance.m_oLevelConfig.PassCondition.LaunchCountMax;
    bool isPlayTimeReachedMax = GameStatusInstance.getPlayTime() >= PlayTimeMax;
    bool isLaunchCountReachedMax = GameStatusInstance.getLaunchCount() == LaunchCountMax && !player->isMoving();
    if (player->isMissReachedMax() || isLaunchCountReachedMax || isPlayTimeReachedMax) {
        CCLOG("Stage Fail!");
        GameStatusInstance.setPass(false);
        m_bSceneFinished = true;
    } else if (m_pMapLayer->isStageClear()) {
        CCLOG("Stage Clear!");
        GameStatusInstance.setPass(true);
        m_bSceneFinished = true;
    }
    
    // when stage finished, move to next scene
    if (m_bSceneFinished) {
        // disable touch
        m_pPlayerLayer->setTouchEnabled(false);
        // stop PlayTime counting and update
        this->unscheduleAllSelectors();
        // pause all map elements
        QIXCocos2dHelper::pauseMapElementsOfCurrentScene();
        // show result intro in some time
        m_pIntroLayer->showResultIntroAfterSomeTime(0.5f);
    }
}

MapArea* GameScene::getMapArea() {
    return m_pMapLayer->getMapArea();
}

void GameScene::initializeBackground() {
    const char* edgeBackground = QIXResourceInstance.m_oGlobalConfig.EdgeBackgroundImage.c_str();
    const char* playerBackground = QIXResourceInstance.m_oGlobalConfig.PlayerBackgroundImage.c_str();
    // create edge background
    CCRect rectEdge = QIXLayout::getBackgroundRect();
    m_pEdgeBackground = QIXCocos2dHelper::createSpriteWithRect(edgeBackground, rectEdge, kScaleExactFit);
    this->addChild(m_pEdgeBackground);
    
    // create player background
    CCRect rectPlayer = QIXLayout::getPlayerAreaRect();
    m_pPlayerBackground = QIXCocos2dHelper::createSpriteWithRect(playerBackground, rectPlayer, kScaleExactFit);
    this->addChild(m_pPlayerBackground);
}


void GameScene::registerNotificationMessage() {
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(GameScene::receiveNotificationMessage),
                                                                  CCNC_MSG_INTRO_PANEL_TOUCHED,
                                                                  NULL);
}

void GameScene::unregisterNotificationMessage() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, CCNC_MSG_INTRO_PANEL_TOUCHED);
}

void GameScene::receiveNotificationMessage(cocos2d::CCObject *obj) {
    CCLOG("Receive message {%s}", CCNC_MSG_INTRO_PANEL_TOUCHED);
    
    // resume
    QIXCocos2dHelper::resumeCurrentScene();
    
    if (m_bSceneFinished) {
        // go to next when intro panel disappeared
        QIXScene::goToGameEndScene();
    } else {
        // enable touch
        m_pPlayerLayer->setTouchEnabled(true);
    }
}