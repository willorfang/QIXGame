//
//  PlayerTrack.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/9/14.
//
//

#include "PlayerTrack.h"

USING_NS_CC;

bool PlayerTrack::init() {
    if (CCNode::init()) {
        // default setting
        m_bWillDrawPlayerTrack = false;
        
        // initialize track points
        int size = QIXResourceInstance.m_oGlobalConfig.PlayerTrackPointsCountDefault;
        m_pPlayerTrackPoints = CCPointArray::create(size);
        m_pPlayerTrackPoints->retain();
        return true;
    }
    return false;
}

PlayerTrack::PlayerTrack() {
    
}

PlayerTrack::~PlayerTrack() {
    CC_SAFE_RELEASE_NULL(m_pPlayerTrackPoints);
}

void PlayerTrack::addPlayerTrackPoint(cocos2d::CCPoint point) {
    if (m_bWillDrawPlayerTrack
        && (m_pPlayerTrackPoints->count() == 0 || !point.equals(m_pPlayerTrackPoints->getControlPointAtIndex(m_pPlayerTrackPoints->count()-1)))) {
        m_pPlayerTrackPoints->addControlPoint(point);
    }
}

void PlayerTrack::clearAllPlayerTrackPoints() {
    while (m_pPlayerTrackPoints->count() != 0) {
        m_pPlayerTrackPoints->removeControlPointAtIndex(0);
    }
}

void PlayerTrack::draw() {
    if (m_bWillDrawPlayerTrack) {
        // draw lines between m_pTrackPoints
        glLineWidth(5.0f);
        ccPointSize(3.0f);
        ccDrawColor4B(0, 255, 255, 255);
        if (m_pPlayerTrackPoints->count() >= 2) {
            // draw the first point
            ccDrawPoint(m_pPlayerTrackPoints->getControlPointAtIndex(0));
            for (int i=0; i<m_pPlayerTrackPoints->count()-1; i++) {
                ccDrawLine(m_pPlayerTrackPoints->getControlPointAtIndex(i), m_pPlayerTrackPoints->getControlPointAtIndex(i+1));
                ccDrawPoint(m_pPlayerTrackPoints->getControlPointAtIndex(i+1));
            }
        }
    }
}