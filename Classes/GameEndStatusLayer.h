//
//  GameEndStatusLayer.h
//  QIXGame
//
//  Created by Wei Fang on 4/14/14.
//
//

#ifndef __QIXGame__GameEndStatusLayer__
#define __QIXGame__GameEndStatusLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "QIXHelper.h"

/**
 @brief Defines the GameEndStatusLayer
 @class GameEndStatusLayer
 
 Defines the GameEndStatusLayer, which includes game status elements
 */
class GameEndStatusLayer : public cocos2d::CCLayer {
public:
    //! create an instance
    CREATE_FUNC(GameEndStatusLayer);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    GameEndStatusLayer();
    //! destruct
    virtual ~GameEndStatusLayer();
    
    //! add status label
    void addGameStatusLabel();
    //! add replay button
    void addReplayButton();
    //! add next button
    void addNextButton();
    //! add score label
    void addScoreLabel();
    //! add score data
    void addScoreData();
    
    //! callback for replay button
    void replayButtonCallBack(cocos2d::CCObject *object,
                              cocos2d::extension::CCControlEvent controlEvent);
    //! callback for next button
    void nextButtonCallBack(cocos2d::CCObject *object,
                              cocos2d::extension::CCControlEvent controlEvent);
    
private:
    //! game status label
    cocos2d::CCLabelBMFont* m_pGameStatusLabel;
    //! game score label
    cocos2d::CCLabelBMFont* m_pScoreLabel;
    //! game score data
    cocos2d::CCSprite* m_pScoreDataNode;
    cocos2d::CCLabelBMFont* m_pScoreData;
    //! replay button
    cocos2d::extension::CCControlButton* m_pReplayButton;
    //! next button
    cocos2d::extension::CCControlButton* m_pNextButton;
};

#endif /* defined(__QIXGame__GameEndStatusLayer__) */
