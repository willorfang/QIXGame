//
//  QIXEffect.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/8/14.
//
//

#include "QIXEffect.h"
#include "QIXLayout.h"
#include "CCNumberChangeAction.h"

USING_NS_CC;

/************************************/
CCActionInterval* QIXEffect::createScaleOutAndBack(int repeat,
                                                   CCCallFuncND* callbackFunc) {
    CCActionInterval* action = NULL;
    
    const float dt = 0.2f;
    const float scaleValue = 1.5f;
    
    CCScaleBy* scaleIn = CCScaleBy::create(dt, scaleValue);
    CCScaleBy* scaleOut = CCScaleBy::create(dt, 1/scaleValue);
    CCSequence* seq = CCSequence::create(scaleIn, scaleOut, NULL);
    
    if (repeat == EFFECT_REPEAT_FOREVER_TIME) {
        action = CCRepeatForever::create(seq);
    } else {
        action = CCSequence::create(CCRepeat::create(seq, repeat), callbackFunc, NULL);
    }
    
    return action;
}

/************************************/
CCActionInterval* QIXEffect::createScaleOutAndBackWithShow(
                                                  int startValue,
                                                  int endValue,
                                                  CCCallFuncND* callbackFunc) {
    CCActionInterval* action = NULL;
    
    const float dt = 0.2f;
    const float unitTime = 0.05f;
    const float maxTime = 1.0f;
    const float scaleValue = 1.5f;
    
    CCScaleBy* scaleIn = CCScaleBy::create(dt, scaleValue);
    float numberChangeDuration = MIN(maxTime, unitTime*(endValue-startValue));
    CCNumberChangeAction* numberChange = CCNumberChangeAction::create("",
                                                                      "%",
                                                                      startValue,
                                                                      endValue,
                                                                      numberChangeDuration);
    CCScaleBy* scaleOut = CCScaleBy::create(dt, 1/scaleValue);
    action =
        (NULL == callbackFunc) ?
        CCSequence::create(scaleIn, numberChange, scaleOut, NULL) :
        CCSequence::create(scaleIn, numberChange, scaleOut, callbackFunc, NULL);
    
    return action;
}

/************************************/
CCActionInterval*  QIXEffect::createSplashShow(CCNode* node, CCCallFuncND* callbackFunc) {
    CCAssert(NULL != node, "");
    CCActionInterval* action = NULL;
    
    const float dt = 0.2f;
    const float scaleValue = 3.0f;
    
    // init scale
    node->setScale(scaleValue);
    
    CCScaleBy* scaleIn = CCScaleBy::create(dt, 1/scaleValue);
    CCFadeIn* fadeIn = CCFadeIn::create(dt);
    CCSpawn* unit = CCSpawn::create(scaleIn, fadeIn, NULL);
    CCDelayTime* delayTime = CCDelayTime::create(dt*3);
    action =
        (NULL == callbackFunc) ?
        CCSequence::create(unit, delayTime, unit->reverse(), NULL):
        CCSequence::create(unit, delayTime, unit->reverse(), callbackFunc, NULL);
    
    return action;
}

/************************************/
CCActionInterval*  QIXEffect::createMoveInOutWithEase(
                                                CCNode* node,
                                                kDirectionType type,
                                                CCCallFuncND* callbackFunc) {
    CCAssert(NULL != node, "");
    CCActionInterval* action = NULL;
    
    // get position
    CCPoint currentPosition = node->getPosition();
    CCPoint screenEdgePosition = node->getPosition();
    CCRect rect = QIXLayout::getScreenRect();
    switch (type) {
        case kDirectionFromTop:
        case kDirectionToTop:
            screenEdgePosition.y = rect.origin.y + rect.size.height + node->boundingBox().size.height;
            break;
        case kDirectionFromBottom:
        case kDirectionToBottom:
            screenEdgePosition.y = rect.origin.y - node->boundingBox().size.height;
            break;
        case kDirectionFromLeft:
        case kDirectionToLeft:
            screenEdgePosition.x = rect.origin.x - node->boundingBox().size.width;
            break;
        case kDirectionFromRight:
        case kDirectionToRight:
            screenEdgePosition.x = rect.origin.x + rect.size.width + node->boundingBox().size.width;
            break;
        default:
            break;
    }
    
    CCPoint targetPosition;
    switch (type) {
        case kDirectionFromTop:
        case kDirectionFromBottom:
        case kDirectionFromLeft:
        case kDirectionFromRight:
            node->setPosition(screenEdgePosition);
            targetPosition = currentPosition;
            break;
            
        case kDirectionToTop:
        case kDirectionToBottom:
        case kDirectionToLeft:
        case kDirectionToRight:
            targetPosition = screenEdgePosition;
            break;
            
        default:
            break;
    }
    
    
    const float dt = 1.0f;
    const float delayTimeMax = 2.0f;
    
    CCDelayTime* delay = CCDelayTime::create(CCRANDOM_0_1()*delayTimeMax);
    CCMoveTo* moveTo = CCMoveTo::create(dt, targetPosition);
    CCEaseBackOut* ease = CCEaseBackOut::create(moveTo);
    
    action =
        (NULL == callbackFunc) ?
        CCSequence::create(delay, ease, NULL):
        CCSequence::create(delay, ease, callbackFunc, NULL);
    
    return action;
}


/************************************/
CCActionInterval*  QIXEffect::createEmphasizeButton(float scaleOrigin, int repeat) {
    CCActionInterval* action = NULL;
    
    const float kShrinkScale = 0.8f;
    const float kShrinkTime = 3.0f;
    const float kLargeScale = 1.4f;
    const float kLargeTime = 0.2f;
    const float kReadyTime = 0.05f;
    
    CCScaleTo* ready = CCScaleTo::create(kReadyTime, scaleOrigin*kShrinkScale);
    CCScaleTo* large = CCScaleTo::create(kLargeTime, scaleOrigin*kLargeScale);
    CCScaleTo* shrink = CCScaleTo::create(kShrinkTime, scaleOrigin);
    CCSequence* seq = CCSequence::create(ready, large, shrink, NULL);
    
    if (repeat == EFFECT_REPEAT_FOREVER_TIME) {
        action = CCRepeatForever::create(seq);
    } else {
        action = CCRepeat::create(seq, repeat);
    }
    
    return action;
}

/************************************/
CCActionInterval*  QIXEffect::createBounceUpAndBack(int repeat) {
    CCActionInterval* action = NULL;
    
    const float bounceTime = 0.5f;
    const float intervalTime = 2.0f;
    const float bounceHeight = 50.0f;
    const float bounceRate = 3.0f;

    CCMoveBy* moveUp = CCMoveBy::create(bounceTime, ccp(0.0f, bounceHeight));
    CCEaseOut* bounceUp = CCEaseOut::create(moveUp, bounceRate);
    CCDelayTime* interval = CCDelayTime::create(intervalTime);
    CCSequence* seq = CCSequence::create(bounceUp, bounceUp->reverse(), interval, NULL);
    
    if (repeat == EFFECT_REPEAT_FOREVER_TIME) {
        action = CCRepeatForever::create(seq);
    } else {
        action = CCRepeat::create(seq, repeat);
    }
    
    return action;
}

/************************************/
CCActionInterval*  QIXEffect::createCreep(int repeat) {
    CCActionInterval* action = NULL;
    
    const float unitTime = 1.0f;
    const float scaleValueX = 1.1f;
    const float scaleValueY = 1.2f;
    const float delayTimeMax = 1.0f;
    
    CCDelayTime* delay = CCDelayTime::create(CCRANDOM_0_1()*delayTimeMax);
    CCScaleBy* scaleForward = CCScaleBy::create(unitTime, scaleValueX, 1/scaleValueY);
    CCScaleBy* scaleBack = CCScaleBy::create(unitTime, 1/scaleValueX, scaleValueY);
    CCSequence* seq = CCSequence::create(delay, scaleForward, scaleBack, NULL);
    
    if (repeat == EFFECT_REPEAT_FOREVER_TIME) {
        action = CCRepeatForever::create(seq);
    } else {
        action = CCRepeat::create(seq, repeat);
    }
    
    return action;
}