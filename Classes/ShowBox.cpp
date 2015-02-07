//
//  ShowBox.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/28/14.
//
//

#include "ShowBox.h"
#include "QIXHelper.h"

USING_NS_CC;

#define LAYOUT_ORDER_BACKGROUND     1
#define LAYOUT_ORDER_CONTENT        2

bool ShowBox::init() {
    if (CCNode::init()) {
        // default setting
        m_pBackgroundNode = NULL;
        m_pContentNode = NULL;
        
        // set content rect
        CCRect rect = CCRectMake(0.0f, 0.0f, 100.0f, 100.0f);
        this->setContentRect(rect);
        return true;
    }
    return false;
}

ShowBox::ShowBox() {
    
}

ShowBox::~ShowBox() {
    
}

void ShowBox::setContentNode(cocos2d::CCNode* node) {
    CCAssert(NULL != node, "");
    if (node != m_pContentNode) {
        if (NULL != m_pContentNode) {
            this->removeChild(m_pContentNode);
        }
        // set rect
        CCSize size = this->getContentSize();
        CCRect rect = CCRectMake(size.width*(-0.5), size.height*(-0.5), size.width, size.height);
        QIXCocos2dHelper::setContentRect(node, rect, kScaleShowAll);
        m_pContentNode = node;
        this->addChild(node, LAYOUT_ORDER_CONTENT);
    }
}

void ShowBox::setBackgroundNode(cocos2d::CCSprite* node) {
    CCAssert(NULL != node, "");
    if (node != m_pBackgroundNode) {
        if (NULL != m_pBackgroundNode) {
            this->removeChild(m_pBackgroundNode);
        }
        // set rect
        CCSize size = this->getContentSize();
        CCRect rect = CCRectMake(size.width*(-0.5), size.height*(-0.5), size.width, size.height);
        QIXCocos2dHelper::setContentRect(node, rect, kScaleShowAll);
        m_pBackgroundNode = node;
        this->addChild(node, LAYOUT_ORDER_BACKGROUND);
    }
}

void ShowBox::setContentRect(const cocos2d::CCRect& rect) {
    this->setContentSize(rect.size);
    this->setPosition(rect.origin);
    
    // deal with the children
    CCRect childRect = CCRectMake(rect.size.width*(-0.5), rect.size.height*(-0.5), rect.size.width, rect.size.height);
    if (NULL != m_pBackgroundNode) {
        QIXCocos2dHelper::setContentRect(m_pBackgroundNode, childRect, kScaleShowAll);
    }
    if (NULL != m_pContentNode) {
        QIXCocos2dHelper::setContentRect(m_pContentNode, childRect, kScaleShowAll);
    }
}