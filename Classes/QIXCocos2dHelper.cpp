//
//  QIXCocos2dHelper.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#include "QIXCocos2dHelper.h"
#include "QIXHelper.h"
#include "B2Node.h"
#include "Box2dWorld.h"
#include "Player.h"

USING_NS_CC;
USING_NS_CC_EXT;

cocos2d::CCLabelBMFont* QIXCocos2dHelper::createWordArt(const char* string, const char* fontFile) {
    CCAssert(NULL != string && NULL != fontFile, "");
    CCLabelBMFont* font = CCLabelBMFont::create(string, fontFile);
    CCAssert(NULL != font, "");
    return font;
}

CCLabelBMFont* QIXCocos2dHelper::createWordArt(const char* string) {
    const char* font = QIXResourceInstance.m_oGlobalConfig.LabelFontDefault.c_str();
    return QIXCocos2dHelper::createWordArt(string, font);
}

cocos2d::CCRect QIXCocos2dHelper::getRectByScale(cocos2d::CCRect& origin, float scale) {
    float width = origin.size.width * scale;
    float height = origin.size.height * scale;
    float x = origin.origin.x + origin.size.width * (1-scale) * 0.5;
    float y = origin.origin.y + origin.size.height * (1-scale) * 0.5;
    return CCRectMake(x, y, width, height);
}

void QIXCocos2dHelper::setContentRect(cocos2d::CCNode* node, const cocos2d::CCRect& rect, kScaleType type) {
    CCAssert(NULL != node, "");
    // set scale
    float scaleX = rect.size.width / node->getContentSize().width;
    float scaleY = rect.size.height / node->getContentSize().height;
    switch (type) {
        case kScaleExactFit:
            break;
            
        case kScaleShowAll:
            scaleX = scaleY = MIN(scaleX, scaleY);
            break;
            
        case kScaleNoBorder:
            scaleX = scaleY = MAX(scaleX, scaleY);
            break;
            
        default:
            break;
    }
    node->setScaleX(scaleX);
    node->setScaleY(scaleY);
    
    // set position to center
    node->setPosition(ccp(rect.origin.x + rect.size.width*0.5, rect.origin.y + rect.size.height*0.5));
}

CCSprite* QIXCocos2dHelper::createSprite(const char *filename) {
    CCAssert(NULL != filename, "");
    CCTexture2D* cache = CCTextureCache::sharedTextureCache()->textureForKey(filename);
    if (NULL == cache) {
        cache = CCTextureCache::sharedTextureCache()->addImage(filename);
    }
    CCAssert(NULL != cache, "");
    CCSprite* sprite  = CCSprite::create();
    CCAssert(NULL != sprite, "");
    sprite->initWithTexture(cache);
    return sprite;
}

CCSprite* QIXCocos2dHelper::createSpriteWithRect(const char *filename, const cocos2d::CCRect& rect, kScaleType type) {
    // create sprite
    CCSprite* sprite  = QIXCocos2dHelper::createSprite(filename);
    // set content rect
    QIXCocos2dHelper::setContentRect(sprite, rect, type);
    return sprite;
}

void QIXCocos2dHelper::pauseCurrentScene() {
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    QIXCocos2dHelper::operateAllSchedulerAndActions(scene, kPauseResumeTypeAllOfRoot, true);
}

void QIXCocos2dHelper::resumeCurrentScene() {
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    QIXCocos2dHelper::operateAllSchedulerAndActions(scene, kPauseResumeTypeAllOfRoot, false);
}

void QIXCocos2dHelper::pauseAllOfRoot(cocos2d::CCNode *root) {
    QIXCocos2dHelper::operateAllSchedulerAndActions(root, kPauseResumeTypeAllOfRoot, true);
}

void QIXCocos2dHelper::resumeAllOfRoot(cocos2d::CCNode *root) {
    QIXCocos2dHelper::operateAllSchedulerAndActions(root, kPauseResumeTypeAllOfRoot, false);
}

void QIXCocos2dHelper::pauseMapElementsOfCurrentScene() {
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    QIXCocos2dHelper::operateAllSchedulerAndActions(scene, kPauseResumeTypeMapElements, true);
}

void QIXCocos2dHelper::resumeMapElementsOfCurrentScene() {
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    QIXCocos2dHelper::operateAllSchedulerAndActions(scene, kPauseResumeTypeMapElements, false);
}

void QIXCocos2dHelper::operateAllSchedulerAndActions(cocos2d::CCNode *root, kPauseResumeType type, bool pause) {
    CCAssert(NULL != root, "");
    
    if (root->isRunning()) {
        switch (type) {
            case kPauseResumeTypeAllOfRoot:
                if (pause) {
                    root->pauseSchedulerAndActions();
                } else {
                    root->resumeSchedulerAndActions();
                }
                break;
               
            case kPauseResumeTypeMapElements:
                // stop if it is Box2DWorld or B2Node
                if (NULL != dynamic_cast<B2Node*>(root)
                    || NULL != dynamic_cast<Box2DWorld*>(root)) {
                    if (pause) {
                        root->pauseSchedulerAndActions();
                    } else {
                        root->resumeSchedulerAndActions();
                    }
                }
                break;
                
            default:
                break;
        }
        
        // deal with children
        CCArray *array = root->getChildren();
        if (NULL != array && array->count() > 0) {
            CCObject *object = NULL;
            CCARRAY_FOREACH(array, object) {
                CCNode *child = dynamic_cast<CCNode*>(object);
                QIXCocos2dHelper::operateAllSchedulerAndActions(child, type, pause);
            }
        }
    }
}

void QIXCocos2dHelper::configLabel(CCLabelBMFont*& label, const char* labelName, const LabelConfig& config) {
    label = QIXCocos2dHelper::createWordArt(labelName, config.Font.c_str());
    // set rect
    CCPoint pos = config.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    CCRect rect = CCRectMake(pos.x - config.Size.width*0.5,
                             pos.y - config.Size.height*0.5,
                             config.Size.width,
                             config.Size.height);
    QIXCocos2dHelper::setContentRect(label, rect, kScaleNoBorder);
}

void QIXCocos2dHelper::configButton(CCControlButton*& button, const char* buttonLabel, const ButtonConfig& config) {
    const char * normalImage = QIXResourceInstance.m_oGlobalConfig.ButtonNormalImage.c_str();
    const char * disabledImage = QIXResourceInstance.m_oGlobalConfig.ButtonDisabledImage.c_str();
    const char * selectedImage = QIXResourceInstance.m_oGlobalConfig.ButtonSelectedImage.c_str();
    const char * highlightedImage = QIXResourceInstance.m_oGlobalConfig.ButtonHighlightedImage.c_str();
    const char * fontName = QIXResourceInstance.m_oGlobalConfig.ButtonFontDefault.c_str();
    CCPoint pos = config.Position;
    QIXLayout::getAbsolutePositionWithScreenRect(pos);
    CCSize size = config.Size;
    
    // set label
    CCLabelBMFont* label = QIXCocos2dHelper::createWordArt(buttonLabel, fontName);
    // set size
    // factor 10: scale according to height instead of width
    CCRect rect = CCRectMake(pos.x - size.width*0.5,
                             pos.y - size.height*0.5*BUTTON_LABEL_SPRITE_RATIO,
                             size.width,
                             size.height*BUTTON_LABEL_SPRITE_RATIO);
    QIXCocos2dHelper::setContentRect(label, rect, kScaleShowAll);
    
    // set buttong background
    CCSize originSize = QIXCocos2dHelper::createSprite(normalImage)->getContentSize();
    int inset = 1.0f;
    CCRect fullRect = CCRectMake(0, 0, originSize.width, originSize.height);
    CCRect insetRect = CCRectMake(inset, inset, originSize.width - inset*2, originSize.height - inset*2);
    CCScale9Sprite* normalSprite = CCScale9Sprite::create(normalImage, fullRect, insetRect);
    CCScale9Sprite* disabledSprite = CCScale9Sprite::create(disabledImage, fullRect, insetRect);
    CCScale9Sprite* selectedSprite = CCScale9Sprite::create(selectedImage, fullRect, insetRect);
    CCScale9Sprite* highlightedSprite = CCScale9Sprite::create(highlightedImage, fullRect, insetRect);
    button = CCControlButton::create(label, normalSprite);
    button->setBackgroundSpriteForState(disabledSprite, CCControlStateDisabled);
    button->setBackgroundSpriteForState(selectedSprite, CCControlStateSelected);
    button->setBackgroundSpriteForState(highlightedSprite, CCControlStateHighlighted);
    // set size
    button->setPreferredSize(size);
    // set position
    button->setPosition(pos);
}