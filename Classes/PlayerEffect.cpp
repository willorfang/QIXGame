//
//  PlayerEffect.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#include "PlayerEffect.h"
#include "QIXHelper.h"

USING_NS_CC;

bool PlayerEffect::init()
{
    if (CCNode::init()) {
        return true;
    }
    return false;
}

PlayerEffect::PlayerEffect() {
    
}

PlayerEffect::~PlayerEffect() {

}

void PlayerEffect::runDefaultFontEffect(std::string content, cocos2d::CCPoint point) {
    // create label
    CCLabelBMFont* label = QIXCocos2dHelper::createWordArt(content.c_str());
    label->setPosition(point);
    this->addChild(label);

    // run the action
    CCCallFuncND* callbackFunc = CCCallFuncND::create(this, callfuncND_selector(PlayerEffect::defaultCallFunc), NULL);
    CCActionInterval* action = QIXEffect::createSplashShow(label, callbackFunc);
    label->runAction(action);
}

void PlayerEffect::runMissEffect(cocos2d::CCPoint point) {
    this->runDefaultFontEffect("Miss", point);
}

void PlayerEffect::runClearEffect(cocos2d::CCPoint point, int ratio) {
    const char* str = CCString::createWithFormat("%d%%", ratio)->getCString();
    this->runDefaultFontEffect(str, point);
}

void PlayerEffect::runSkipEffect(cocos2d::CCPoint point) {
    this->runDefaultFontEffect("Skip", point);
}

void PlayerEffect::runStepEffect(cocos2d::CCPoint point) {
    this->runDefaultFontEffect("Step", point);
}

void PlayerEffect::defaultCallFunc(cocos2d::CCNode *node, void* data) {
    CCAssert(NULL != node, "");
    node->removeFromParent();
}

void PlayerEffect::runChangeStateEffect(cocos2d::CCNode* node) {
    CCAssert(NULL != node, "");
    // set scale to default
    node->stopAllActions();
    node->setScale(1.0f);
    // run animation
    CCActionInterval* action = QIXEffect::createScaleOutAndBack();
    node->runAction(action);
}