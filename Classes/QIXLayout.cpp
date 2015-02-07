//
//  QIXLayout.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#include "QIXLayout.h"
#include "QIXHelper.h"

USING_NS_CC;

cocos2d::CCRect QIXLayout::getWorldBoundrayRect() {
    return QIXLayout::getPlayerAreaRect();
}

cocos2d::CCRect QIXLayout::getScreenRect() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visiableOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCRect rect = CCRectMake(visiableOrigin.x, visiableOrigin.y,
                             visibleSize.width, visibleSize.height);
    return rect;
}

cocos2d::CCRect QIXLayout::getBackgroundRect() {
    return QIXLayout::getScreenRect();
}

cocos2d::CCRect QIXLayout::getPlayerAreaRect() {
    // get parent rect
    CCSize visibleSize = QIXLayout::getBackgroundRect().size;
    CCPoint visiableOrigin = QIXLayout::getBackgroundRect().origin;
    
    CCSize edgeSize = QIXResourceInstance.m_oGlobalConfig.ItemSizeDefault;
    float topPanelHeight = QIXLayout::getTopPanelRect().size.height;
    float bottomPanelHeight = QIXLayout::getBottomPanelRect().size.height;
    
    float x_start = visiableOrigin.x + edgeSize.width;
    float y_start = visiableOrigin.y + bottomPanelHeight;
    float x_end = visiableOrigin.x + visibleSize.width - edgeSize.width;
    float y_end = visiableOrigin.y + visibleSize.height - topPanelHeight;
    CCRect rect = CCRectMake(x_start, y_start, x_end-x_start, y_end-y_start);
    return rect;
}

cocos2d::CCRect QIXLayout::getMapAreaRect() {
    // get parent rect
    CCSize visibleSize = QIXLayout::getPlayerAreaRect().size;
    CCPoint visiableOrigin = QIXLayout::getPlayerAreaRect().origin;
    
    CCSize edgeSize = QIXResourceInstance.m_oGlobalConfig.ItemSizeDefault;
    float x_start = visiableOrigin.x + edgeSize.width;
    float y_start = visiableOrigin.y + edgeSize.height;
    float x_end = visiableOrigin.x + visibleSize.width - edgeSize.width;
    float y_end = visiableOrigin.y + visibleSize.height - edgeSize.height;
    CCRect rect = CCRectMake(x_start, y_start, x_end-x_start, y_end-y_start);
    return rect;
}

cocos2d::CCRect QIXLayout::getTopPanelRect() {
    // get parent rect
    CCSize visibleSize = QIXLayout::getBackgroundRect().size;
    CCPoint visiableOrigin = QIXLayout::getBackgroundRect().origin;
    
    CCSize edgeSize = QIXResourceInstance.m_oGlobalConfig.ItemSizeDefault;
    float x_start = visiableOrigin.x;
    float y_start = visiableOrigin.y + visibleSize.height - edgeSize.height * PANEL_HEIGHT_SCALE_VALUE;
    float x_end = visiableOrigin.x + visibleSize.width;
    float y_end = visiableOrigin.y + visibleSize.height;
    CCRect rect = CCRectMake(x_start, y_start, x_end-x_start, y_end-y_start);
    return rect;
}

cocos2d::CCRect QIXLayout::getBottomPanelRect() {
    // get parent rect
    CCSize visibleSize = QIXLayout::getBackgroundRect().size;
    CCPoint visiableOrigin = QIXLayout::getBackgroundRect().origin;
    
    CCSize edgeSize = QIXResourceInstance.m_oGlobalConfig.ItemSizeDefault;
    float x_start = visiableOrigin.x;
    float y_start = visiableOrigin.y;
    float x_end = visiableOrigin.x + visibleSize.width;
    float y_end = visiableOrigin.y + edgeSize.height * PANEL_HEIGHT_SCALE_VALUE;
    CCRect rect = CCRectMake(x_start, y_start, x_end-x_start, y_end-y_start);
    return rect;
}

cocos2d::CCPoint QIXLayout::getScreenCenter() {
    CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visiableOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    return ccp(visiableOrigin.x + visiableSize.width*0.5,
               visiableOrigin.y + visiableSize.height*0.5);
}

void QIXLayout::changeToAbsolutePositionFromRelative(cocos2d::CCPoint& origin,
                                          const cocos2d::CCRect& area) {
    if (origin.x < 1.0f) {
        origin.x = area.origin.x + area.size.width * origin.x;
    }
    if (origin.y < 1.0f) {
        origin.y = area.origin.x + area.size.height * origin.y;
    }
}

void QIXLayout::getAbsolutePositionWithScreenRect(cocos2d::CCPoint& origin) {
    // compute in the WorldBoundaryRect coordinate
    QIXLayout::changeToAbsolutePositionFromRelative(origin,
                                                    QIXLayout::getScreenRect());
}
