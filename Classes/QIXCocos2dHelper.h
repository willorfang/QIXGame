//
//  QIXCocos2dHelper.h
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#ifndef __QIXGame__QIXCocos2dHelper__
#define __QIXGame__QIXCocos2dHelper__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "QIXMacros.h"
#include "QIXResource.h"

typedef enum {
    kPauseResumeTypeAllOfRoot = 0,
    kPauseResumeTypeMapElements
} kPauseResumeType;

/**
 @brief Defines a amount of cocos2d related help functions
 @class QIXCocos2dHelper
 
 Defines a amount of cocos2d related help functions
 */
class QIXCocos2dHelper : public cocos2d::CCObject
{
public:
    //! set the content rect of the CCNode
    static void setContentRect(cocos2d::CCNode* node, const cocos2d::CCRect& rect, kScaleType type);
    
    //! create sprite according to CCRect
    static cocos2d::CCSprite* createSprite(const char *filename);
    static cocos2d::CCSprite* createSpriteWithRect(const char *filename, const cocos2d::CCRect& rect, kScaleType type);
    
    //! pause/resume schedulers and actions
public:
    static void pauseCurrentScene();
    static void resumeCurrentScene();
    static void pauseAllOfRoot(cocos2d::CCNode *root);
    static void resumeAllOfRoot(cocos2d::CCNode *root);
    static void pauseMapElementsOfCurrentScene();
    static void resumeMapElementsOfCurrentScene();
protected:
    static void operateAllSchedulerAndActions(cocos2d::CCNode *root,
                                              kPauseResumeType type,
                                              bool pause);

public:
    //! create wordart
    static cocos2d::CCLabelBMFont* createWordArt(const char* string);
    static cocos2d::CCLabelBMFont* createWordArt(const char* string, const char* fontFile);
    
    //! get a new CCRect by scaling from the origin one
    static cocos2d::CCRect getRectByScale(cocos2d::CCRect& origin, float scale);

    //! config a label according to config
    static void configLabel(cocos2d::CCLabelBMFont*& label, const char* labelName, const LabelConfig& config);
    
    //! config a button according to config
    static void configButton(cocos2d::extension::CCControlButton*& button, const char* buttonLabel, const ButtonConfig& config);
};


#endif /* defined(__QIXGame__QIXCocos2dHelper__) */
