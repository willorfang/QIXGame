//
//  QIXB2ContactListener.h
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#ifndef __QIXGame__QIXB2ContactListener__
#define __QIXGame__QIXB2ContactListener__

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "QIXMacros.h"
#include "B2Node.h"
#include <set>

class GameScene;

/**
 @brief Defines Box2D contact listener
 @class QIXB2ContactListener
 
 Defines Box2D contact listener
 */
class QIXB2ContactListener : public cocos2d::CCObject, public b2ContactListener {
public:
    //! create an instance
    CREATE_FUNC(QIXB2ContactListener);
    
    //! listener functions
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    //! deal with collisions using delegate
    void DealWithCollisions();
    
    //! delegate to deal with contacts
    CC_SYNTHESIZE(GameScene*, m_pDelegate, Delegate);
    
protected:
    //! initialize
    virtual bool init() {
        return true;
    }
    //! construct
    QIXB2ContactListener();
    //! destruct
    virtual ~QIXB2ContactListener();
    
    //! structure to record a contact
    typedef struct {
        B2Node* nodeA;
        B2Node* nodeB;
    } ContactItem;
    
    //! contact state
    typedef enum {
        kContactStateCreated = 0,
        kContactStateProcessed,
        kContactStateShouldDelete
    } ContactState;
    
    //! compare function
    /**
     ATTENTION: compare pointer instead of B2Node-related attribute considering deleted B2Node object
     */
    struct ContactItemCompare {
        bool operator()(const ContactItem& itemA, const ContactItem& itemB) const {
            if (itemA.nodeA != itemB.nodeA) {
                return itemA.nodeA < itemB.nodeA;
            }
            if (itemA.nodeB != itemB.nodeB) {
                return itemA.nodeB < itemB.nodeB;
            }
            return  false;
        }
    };
    
    //! structure to record all contact info
    typedef std::map<ContactItem, ContactState, ContactItemCompare> ContactMap;
    
    //! add a contact
    void addContact(b2Contact* contact);
    
    //! remove a contact
    void removeContact(b2Contact* contact);
    
    //! register notification message
    void registerNotificationMessage();
    //! unregister notification message
    void unregisterNotificationMessage();
    //! callback for notification
    void receiveNotificationMessage(cocos2d::CCObject *obj);

private:
    //! recode all the contacts
    ContactMap m_vContacts;
};

#endif /* defined(__QIXGame__QIXB2ContactListener__) */
