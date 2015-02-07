//
//  PanelLayer.h
//  QIXGame
//
//  Created by Wei Fang on 5/7/14.
//
//

#ifndef __QIXGame__PanelLayer__
#define __QIXGame__PanelLayer__

#include "cocos2d.h"
#include "QIXMacros.h"

#include <stack>

/**
 @brief Defines the panel layer
 @class PanelLayer
 
 Defines the panel layer
 */
class PanelLayer : public cocos2d::CCLayer
{
public:
    //! create an instance
    CREATE_FUNC(PanelLayer);
    
    //! update panel label
    void setEnemyCount(int count);
    void setLaunchCount(int count);
    void setPlayTime(int count);
    void setAreaRatio(int startValue, int endValue);
    
    //! decrease a life
    void decreaseOneLife();
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    PanelLayer();
    //! destruct
    virtual ~PanelLayer();
    
    //! run default font effect
    void runDefaultFontEffect(cocos2d::CCLabelBMFont* label);
    //! stop default font effect
    void stopDefaultFontEffect(cocos2d::CCLabelBMFont* label);
    //! reset default font effect
    void resetDefaultFontEffect(cocos2d::CCLabelBMFont* label);
    //! default Callback func
    void defaultFontCallFunc(cocos2d::CCNode *node, void* data);
    
    //! initialize remaining life
    void initializeRemaingLife();
    //! run delayed bounce forever effect
    void runDelayedBounceForeverEffect(cocos2d::CCSprite* sprite, float delay);
    void delayedBounceForeverCallFunc(cocos2d::CCNode *node);
    
    //! get panel layout
    std::vector<cocos2d::CCRect> getTopPanelLayout(int itemCount);
    std::vector<cocos2d::CCRect> getBottomPanelLayout(int itemCount);
    
private:
    //! panel elements
    cocos2d::CCLabelBMFont* m_pEnemyCount;
    cocos2d::CCLabelBMFont* m_pLaunchCount;
    cocos2d::CCLabelBMFont* m_pAreaRatio;
    cocos2d::CCLabelBMFont* m_pPlayTime;
    cocos2d::CCSprite* m_pTopPanel;
    cocos2d::CCSprite* m_pBottomPanel;
    std::stack<cocos2d::CCSprite*> m_sRemainingLife;
    //! default scale
    float m_fDefaultScale;
};

#endif /* defined(__QIXGame__PanelLayer__) */
