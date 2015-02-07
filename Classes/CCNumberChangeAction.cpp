//
//  CCNumberChangeAction.cpp
//  QIXGame
//
//  Created by Wei Fang on 6/2/14.
//
//

#include "CCNumberChangeAction.h"

CCNumberChangeAction* CCNumberChangeAction::create(std::string prefix, std::string suffix, int start, int end, float duration) {
    CCNumberChangeAction* action = new CCNumberChangeAction();
    action->init(prefix, suffix, start, end, duration);
    action->autorelease();
    
    return action;
}

bool CCNumberChangeAction::init(std::string prefix, std::string suffix, int start, int end, float duration) {
    if (CCActionInterval::initWithDuration(duration)) {
        m_sPrefix = prefix;
        m_sSuffix = suffix;
        m_iStartNumber = start;
        m_iendNumber = end;
        return true;
    }
    return false;
}

void CCNumberChangeAction::update(float time) {
    if (CCLabelBMFont* target = dynamic_cast<CCLabelBMFont*>(m_pTarget)) {
        int step = (m_iendNumber-m_iStartNumber) * time;
        int showNumber = m_iStartNumber + step;
        const char* str = CCString::createWithFormat("%s%d%s",
                                                     m_sPrefix.c_str(),
                                                     showNumber,
                                                     m_sSuffix.c_str())->getCString();
        target->setString(str);
    }
}