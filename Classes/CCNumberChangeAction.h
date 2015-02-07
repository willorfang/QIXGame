//
//  CCNumberChangeAction.h
//  QIXGame
//
//  Created by Wei Fang on 6/2/14.
//
//

#ifndef __QIXGame__CCNumberChangeAction__
#define __QIXGame__CCNumberChangeAction__

#include "cocos2d.h"

USING_NS_CC;

/**
 @brief Defines a CCAction for number change
 @class CCNumberChangeAction
 
 Defines a CCAction for number change
 */
class CCNumberChangeAction : public CCActionInterval {
public:
    //! create an instance
    static CCNumberChangeAction* create(std::string prefix, std::string suffix, int start, int end, float duration);
    //! update
    virtual void update(float time);
    
protected:
    //! initialize
    virtual bool init(std::string prefix, std::string suffix, int start, int end, float duration);
    
private:
    //! arguments
    std::string m_sPrefix;
    std::string m_sSuffix;
    int m_iStartNumber;
    int m_iendNumber;
};

#endif /* defined(__QIXGame__CCNumberChangeAction__) */
