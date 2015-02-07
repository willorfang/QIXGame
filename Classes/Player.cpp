//
//  Player.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/2/14.
//
//

#include "Player.h"
#include "QIXHelper.h"

#include "PlayerLayer.h"
#include "PanelLayer.h"
#include "MapLayer.h"
#include "MapArea.h"
#include "GameScene.h"
#include "PlayerEffect.h"

USING_NS_CC;

Player* Player::m_pInstance = NULL;

bool Player::init() {
    MapElementConfig config;
    config.Image = QIXResourceInstance.m_oGlobalConfig.PlayerSafeImage;
    config.Position = QIXResourceInstance.m_oLevelConfig.Player.Position;
    config.Size = QIXResourceInstance.m_oGlobalConfig.PlayerSize;
    config.Type = "Player";
    
    if (B2Sprite::init(config)) {
        // initialize state sprites
        this->initializeStateSprites();
        
        // initialize step show box
        this->initialzieStepShowBox();
        
        // default settings
        m_iMissCount = 0;
        this->setBodyType(b2_dynamicBody);
        this->setRole(kRoleTypePlayer);
        // normal mass & small friction & normal collision & add damping
        this->setPhysicalParameter(10.0f, 0.3f, 0.7f, 0.5f, 200.0f);
        // set fixed rotation
        this->setFixedRotation(true);
        // reset
        this->reset();
        
        // add player path
        m_pPlayerPath = PlayerPath::create();
        CCAssert(NULL != m_pPlayerPath, "");
        this->addChild(m_pPlayerPath);
        
        // add into box2d world in the end
        this->addIntoBox2DWorld(this->getPosition());
        
        // register update()
        this->scheduleUpdate();
        return true;
    }
    return false;
}

void Player::initializeStateSprites() {
    const char* actionImage = QIXResourceInstance.m_oGlobalConfig.PlayerActionImage.c_str();
    CCSize itemSize = QIXResourceInstance.m_oGlobalConfig.PlayerSize;
    
    // for safe state sprite reference
    m_pPlayerSafeSprite = static_cast<CCSprite*>(m_pNode);
    
    // add action state sprite
    m_pPlayerActionSprite = QIXCocos2dHelper::createSprite(actionImage);
    CCRect rect = CCRectMake(itemSize.width*(-0.5), itemSize.height*(-0.5), itemSize.width, itemSize.height);
    QIXCocos2dHelper::setContentRect(m_pPlayerActionSprite, rect, kScaleShowAll);
    this->addChild(m_pPlayerActionSprite);
}

void Player::initialzieStepShowBox() {
    const char* stepShowboxImage = QIXResourceInstance.m_oGlobalConfig.PlayerStepShowboxImage.c_str();
    CCSize itemSize = QIXResourceInstance.m_oGlobalConfig.PlayerSize;
    
    // add step show box
    m_pStepShowBox = ShowBox::create();
    CCSprite* showBoxBackground = QIXCocos2dHelper::createSprite(stepShowboxImage);
    CCLabelBMFont* showBoxLabel = QIXCocos2dHelper::createWordArt("0");
    m_pStepShowBox->setBackgroundNode(showBoxBackground);
    m_pStepShowBox->setContentNode(showBoxLabel);
    CCRect rect = CCRectMake(itemSize.width*0.7, itemSize.height*0.7, itemSize.width, itemSize.height);
    m_pStepShowBox->setContentRect(rect);
    this->addChild(m_pStepShowBox);
}

void Player::updateStepShowBoxLabel(const char *str) {
    CCLabelBMFont* label = static_cast<CCLabelBMFont*>(m_pStepShowBox->getContentNode());
    CCAssert(NULL != label, "");
    label->setString(str);
}

void Player::updateStepShowBoxLabel(int stepNum) {
    const char *label = CCString::createWithFormat("%d", stepNum)->getCString();
    this->updateStepShowBoxLabel(label);
}


Player::Player() {
    
}

Player::~Player() {
    // avoid dangling pointer
    m_pInstance = NULL;
}

void Player::setInitSprite() {
    this->setSafeSprite();
}

void Player::setSafeSprite() {
    m_pPlayerSafeSprite->setVisible(true);
    m_pPlayerActionSprite->setVisible(false);
    m_bIsSafety = true;
    this->setSensor(true);
    // reset step showbox label
    this->updateStepShowBoxLabel("0");
}

void Player::setSafeSpriteWithAnimation() {
    this->setSafeSprite();
    // run state change effect
    this->getPlayerEffect()->runChangeStateEffect(this);
}

void Player::setActionSprite() {
    m_pPlayerSafeSprite->setVisible(false);
    m_pPlayerActionSprite->setVisible(true);
    m_bIsSafety = false;
    this->setSensor(false);
}

void Player::setActionSpriteWithAnimation() {
    this->setActionSprite();
    // run state change effect
    this->getPlayerEffect()->runChangeStateEffect(this);
}

void Player::reset() {
    // set position
    cocos2d::CCPoint pos = QIXResourceInstance.m_oLevelConfig.Player.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    this->setPosition(pos);
    this->setRotation(0.0f);
    this->setVelocity(CCPointZero);
    this->setAngularVelocity(0.0f);
    this->setScale(1.0f);
    this->setInitSprite();
    
    // reset state data
    m_vPrevPosition = this->getPosition();
    m_bIsPrevMoving = false;
    // reset to active
    m_bIsActive = true;
}

bool Player::canBeAttracted() {
    // can be attracted when it is not safety
    return !m_bIsSafety;
}

void Player::beKilled() {
    CCLOG("Player is killed");
    
    if (m_bIsActive) {
        m_bIsActive = false;
    } else {
        // when conllide with serveral enemies at the same time
        // ignore the latter ones
        return;
    }
    
    /**
     Attention: should before player's animation run.
     Else, the player's animation itself will not run,
     so all map elements will not resume forever
     */
    // freeze all map elements
    QIXCocos2dHelper::pauseMapElementsOfCurrentScene();
    
    // disable touch
    this->getPlayerLayer()->setTouchEnabled(false);
    
    // leave from b2World
    this->removeFromBox2DWorld();
    
    // update panel
    this->getPanelLayer()->decreaseOneLife();
    
    // kill animation
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCBlink* blink = CCBlink::create(0.5f, 8);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Player::missCallFunc));
    CCSequence* seq = CCSequence::create(delay, blink, callFunc, NULL);
    this->runAction(seq);
    
    // run effect
    this->getPlayerEffect()->runMissEffect(this->getPosition());
    QIXAudioEngineInstance.playPlayerKilled();
}

void Player::missCallFunc() {
    // update miss HERE
    ++m_iMissCount;
    // un-freeze all map alements
    QIXCocos2dHelper::resumeMapElementsOfCurrentScene();
    // reset to default
    this->reset();
    // add into b2World
    this->addIntoBox2DWorld(this->getPosition());
    // enable touch
    this->getPlayerLayer()->setTouchEnabled(true);
}

void Player::beAttracted(const cocos2d::CCPoint &point) {
    CCLOG("Player is attracted");
    
    if (m_bIsActive) {
        m_bIsActive = false;
    } else {
        // when conllide with serveral enemies at the same time
        // ignore the latter ones
        return;
    }
    
    // disable touch
    this->getPlayerLayer()->setTouchEnabled(false);
    
    // leave from b2World
    this->removeFromBox2DWorld();

    // update panel
    this->getPanelLayer()->decreaseOneLife();
    
    // move to hole
    CCMoveTo* moveTo = CCMoveTo::create(0.3, point);
    CCScaleTo* scaleTo = CCScaleTo::create(0.3, 0.5);
    CCFadeOut* fadeOut = CCFadeOut::create(0.3);
    CCSpawn* uniteAction = CCSpawn::create(moveTo, scaleTo, fadeOut, NULL);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Player::missCallFunc));
    CCFiniteTimeAction* seq = CCSequence::create(uniteAction, callFunc, NULL);
    this->runAction(seq);
    
    // run effect
    this->getPlayerEffect()->runMissEffect(this->getPosition());
    QIXAudioEngineInstance.playPlayerKilled();
}

void Player::increaseClearCount(int clearedRatio) {
    // update panel
    int newValue = this->getMapLayer()->getAreaClearRatio();
    int oldValue = newValue - clearedRatio;
    this->getPanelLayer()->setAreaRatio(oldValue, newValue);
    
    // run effect
    this->getPlayerEffect()->runClearEffect(this->getPosition(), clearedRatio);
    QIXAudioEngineInstance.playPlayerClear();
}

void Player::update(float dt) {
    // the map area
    MapArea* area = MapArea::GetTheMapArea();
    // get position
    CCPoint prevPosition = this->getPrevPosition();
    CCPoint currentPosition = this->getPosition();
    // the intersection point between (prevPosition, currentPosition) and the map area
    CCPoint intersectionPoint = CCPointZero;
    // whether player is in map area in the previous frame
    bool isPrevPlayerInArea = area->containsPoint(prevPosition);
    // whether player is in map area in the current frame
    bool isCurrentPlayerInArea = area->containsPoint(currentPosition);
    
    /** update player's velocity manullay
     */
    this->updateVelocity();
    
    // avoid small velocity
    if (!this->isMoving() && ccpLength(this->getVelocity()) >= 0) {
        this->setVelocity(CCPointZero);
    }
    
    /** add path point
        1) the player is in map area
        2) the player change from moving to non-moving
     */
    if (isCurrentPlayerInArea && this->getIsPrevMoving() && !this->isMoving()) {
        m_pPlayerPath->addMiddlePathPoint(currentPosition);
    }
    // update state data
    m_bIsPrevMoving = this->isMoving();
    
    
    /** player miss when
        1) its lineCount has reached PLAYER_PATH_SIZE_VALUE - 1
        2) it has failed to clear map area
     */
    if (isCurrentPlayerInArea
        && (m_pPlayerPath->isPathReached() || m_pPlayerPath->isPathExceeded()) ) {
        // kill the player
        this->getPlayerLayer()->resetPlayer();
        return;
    }
    
    /** deal with the cases when
     1) the player has moved out of the map area
     2) the player has moved into the map area
     */
    if (isPrevPlayerInArea && !isCurrentPlayerInArea) {
        // update player's state sprite
        this->setSafeSprite();
        
        // get intersection point
        intersectionPoint = area->getInterSectionPoint(prevPosition, currentPosition);
        // clear player track
        this->getPlayerTrack()->endTrack();
        // add the last path point
        m_pPlayerPath->addLastPathPoint(intersectionPoint);
        // when player's path reached, then clear map area
        if (m_pPlayerPath->isPathReached()) {
            // get player's path
            QIXPointSet path = m_pPlayerPath->getPathPoints();
            // update map area
            int flag = area->updateBoundaryPoints(path);
            if (flag != UNSIGNED_INT_VALUE_ILLEGAL) {
                // update player return
                this->increaseClearCount(flag);
            } else {
                // run skip effect
                this->getPlayerEffect()->runSkipEffect(currentPosition);
            }
        } else {
            // run skip effect
            this->getPlayerEffect()->runSkipEffect(currentPosition);
        }
        // clear player path
        m_pPlayerPath->clearPathPoints();
    } else if (!isPrevPlayerInArea && isCurrentPlayerInArea) {
        // update player's state sprite
        this->setActionSprite();
        
        // get intersection point
        intersectionPoint = area->getInterSectionPoint(prevPosition, currentPosition);
        // begin to record player path
        m_pPlayerPath->addFirstPathPoint(intersectionPoint);
        // start to draw player track
        this->getPlayerTrack()->beginTrack();
        this->getPlayerTrack()->addPlayerTrackPoint(intersectionPoint);
    }
    
    // update player track
    this->getPlayerTrack()->addPlayerTrackPoint(currentPosition);
    
    // update prev position
    this->setPrevPosition(currentPosition);
}

void Player::onCollisionWithAnotherObject(B2Node* node) {
    switch (node->getRole()) {
        case kRoleTypeStaticEnemy:
        case kRoleTypeLineEnemy:
        case kRoleTypeTrackEnemy:
            if (!m_bIsSafety) {
                this->getPlayerLayer()->resetPlayer();
            }
            break;
            
        case kRoleTypeHoleGrid:
            if (!m_bIsSafety) {
                this->getPlayerLayer()->attractPlayer(node->getPosition());
            }
            break;
            
        case kRoleTypeWallGrid:
            m_pPlayerPath->addMiddlePathPoint(this->getPosition());
            break;
            
        default:
            break;
    }
}

PlayerEffect* Player::getPlayerEffect() {
    return this->getPlayerLayer()->getPlayerEffect();
}

PlayerTrack* Player::getPlayerTrack() {
    return this->getPlayerLayer()->getPlayerTrack();
}

void Player::runStepEffect() {
//    // run step animation
//    PlayerEffect* effect = this->getPlayerEffect();
//    effect->runStepEffect(this->getPosition());
    // update showbox label
    this->updateStepShowBoxLabel(m_pPlayerPath->getPathSize());
}
