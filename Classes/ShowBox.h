//
//  ShowBox.h
//  QIXGame
//
//  Created by Wei Fang on 5/28/14.
//
//

#ifndef __QIXGame__ShowBox__
#define __QIXGame__ShowBox__

#include "cocos2d.h"

/**
 @brief Defines the showbox
 @class ShowBox
 
 Defines the showbox
 */
class ShowBox : public cocos2d::CCNode {
public:
    //! create an instance
    CREATE_FUNC(ShowBox);
    
    //! set content node
    void setContentNode(cocos2d::CCNode* node);
    cocos2d::CCNode* getContentNode() {
        return m_pContentNode;
    }
    //! set background node
    void setBackgroundNode(cocos2d::CCSprite* node);
    //! set Rect
    void setContentRect(const cocos2d::CCRect& rect);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    ShowBox();
    //! destruct
    virtual ~ShowBox();

private:
    //! content node
    cocos2d::CCNode* m_pContentNode;
    //! content node
    cocos2d::CCSprite* m_pBackgroundNode;
};

#endif /* defined(__QIXGame__ShowBox__) */
