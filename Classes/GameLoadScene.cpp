//
//  GameLoadScene.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/16/14.
//
//

#include "GameLoadScene.h"
#include "GameLoginScene.h"

USING_NS_CC;
using namespace std;

#define LAYOUT_ORDER_BACKGROUND             0
#define LAYOUT_ORDER_LABEL_BACKGROUND       1
#define LAYOUT_ORDER_LABEL                  2

#define TIME_INTERVAL_MIN                   0.1f

bool GameLoadScene::init() {
    if (CCScene::init()) {
        // init scene
        this->addBackground();
        this->addTitleLabel();
        this->addLoadingLabel();
        this->addProgressTimerBackgournd();
        this->addProgressTimer();
        // load resources
        this->scheduleOnce(schedule_selector(GameLoadScene::loadResources), TIME_INTERVAL_MIN);
        // update progress
        this->scheduleUpdate();
        
        return true;
    }
    return false;
}


void GameLoadScene::onEnterTransitionDidFinish() {
    CCScene::onEnterTransitionDidFinish();
}

void GameLoadScene::onExit() {
    CCScene::onExit();
}

GameLoadScene::GameLoadScene() {
    CCLOG("====================Game Load Scene====================");
    // initialize global objects
    /**
     CAUTION: the global objects should run initialize here IF it contains JNI calls
     */
    QIXAudioEngineInstance.initialize();
    QIXResourceInstance.initialize();
}

GameLoadScene::~GameLoadScene() {
}

void GameLoadScene::update(float dt) {
    int progress = m_sResourceLoadIndicator.loadProgress;
    int total = m_sResourceLoadIndicator.loadTotal;
    float percentage = 100.0f * progress / total;
    m_pLoadProgressTimer->setPercentage(percentage);
//    CCLOG("load process: %f", percentage);
    
    if (progress == total) {
        CCLOG("Resource load ended");
        // stop scheduler
        this->unscheduleAllSelectors();
        // go to next scene
        QIXScene::goToGameLoginScene();
    }
}

void GameLoadScene::addBackground() {
    const char* filename = QIXResourceInstance.m_oLoadSceneConfig.BackgroundImage.c_str();
    CCRect rectEdge = QIXLayout::getBackgroundRect();
    m_pBackground = QIXCocos2dHelper::createSpriteWithRect(filename, rectEdge, kScaleExactFit);
    this->addChild(m_pBackground, LAYOUT_ORDER_BACKGROUND);
}

void GameLoadScene::addProgressTimerBackgournd() {
    const SpriteConfig& config = QIXResourceInstance.m_oLoadSceneConfig.ProgressBarBackground;
    const char* filename = config.Image.c_str();
    
    // set rect
    CCPoint pos = config.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    CCRect rect = CCRectMake(pos.x - config.Size.width*0.5,
                             pos.y - config.Size.height*0.5,
                             config.Size.width,
                             config.Size.height);
    m_pLoadProgressTimerBackground = QIXCocos2dHelper::createSpriteWithRect(filename,
                                                                            rect,
                                                                            kScaleExactFit);
    this->addChild(m_pLoadProgressTimerBackground, LAYOUT_ORDER_LABEL_BACKGROUND);
}

void GameLoadScene::addProgressTimer() {
    const SpriteConfig& config = QIXResourceInstance.m_oLoadSceneConfig.ProgressBar;
    
    const char* filename = config.Image.c_str();
    CCSprite* sprite = QIXCocos2dHelper::createSprite(filename);
    
    m_pLoadProgressTimer = CCProgressTimer::create(sprite);
    // set bar type
    m_pLoadProgressTimer->setType(kCCProgressTimerTypeBar);
    // set start point
    m_pLoadProgressTimer->setMidpoint(ccp(0, 0));
    // set horizontal direction
    m_pLoadProgressTimer->setBarChangeRate(ccp(1, 0));
    // set init percentage
    m_pLoadProgressTimer->setPercentage(0.0f);
    
    // set position
    CCPoint pos = config.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    m_pLoadProgressTimer->setPosition(pos);
    // set size
    CCRect rect = CCRectMake(pos.x - config.Size.width*0.5,
                             pos.y - config.Size.height*0.5,
                             config.Size.width,
                             config.Size.height);
    QIXCocos2dHelper::setContentRect(m_pLoadProgressTimer, rect, kScaleExactFit);
    
    this->addChild(m_pLoadProgressTimer, LAYOUT_ORDER_LABEL);
}

void GameLoadScene::addTitleLabel() {
    const char* name = "QIXGame";
    const LabelConfig& config = QIXResourceInstance.m_oLoadSceneConfig.TitleLabel;
    // config
    QIXCocos2dHelper::configLabel(m_pGameTitleLabel, name, config);
    this->addChild(m_pGameTitleLabel, LAYOUT_ORDER_LABEL);
}

void GameLoadScene::addLoadingLabel() {
    const char* name = "Loading...";
    const LabelConfig& config = QIXResourceInstance.m_oLoadSceneConfig.LoadingLabel;
    // config
    QIXCocos2dHelper::configLabel(m_pLoadingLabel, name, config);
    this->addChild(m_pLoadingLabel, LAYOUT_ORDER_LABEL);
}

void GameLoadScene::loadResources() {
    CCLOG("Resource load began");
    
    const vector<string>& images = QIXResourceInstance.m_vImages;
    const vector<string>& sounds = QIXResourceInstance.m_vSounds;
    m_sResourceLoadIndicator.loadTotal = images.size() + sounds.size();
    m_sResourceLoadIndicator.loadProgress = sounds.size();
    
    // sound
    for (int i=0; i<sounds.size(); ++i) {
        const char* filename = sounds[i].c_str();
        QIXAudioEngineInstance.preloadEffect(filename);
        CCLOG("load sound: %s", filename);
    }
    
    // image
    for (int i=0; i<images.size(); ++i) {
        const char* filename = images[i].c_str();
        CCTextureCache::sharedTextureCache()->addImageAsync(filename,
                                                            this,
                                                            callfuncO_selector(GameLoadScene::loadAllImagesCallbackFunc));
        CCLOG("load image: %s", filename);
    }
}

void GameLoadScene::loadAllImagesCallbackFunc(CCObject* obj) {
    ++m_sResourceLoadIndicator.loadProgress;
}
