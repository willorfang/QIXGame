//
//  QIXB2ContactListener.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/1/14.
//
//

#include "QIXB2ContactListener.h"
#include "GameScene.h"
#include "Player.h"

using namespace std;
USING_NS_CC;

QIXB2ContactListener::QIXB2ContactListener() {
    // register CCNotificationCenter message
    this->registerNotificationMessage();
}

QIXB2ContactListener::~QIXB2ContactListener() {
    // unregister CCNotificationCenter message
    this->unregisterNotificationMessage();
}

void QIXB2ContactListener::addContact(b2Contact* contact) {
    B2Node * nodeA = static_cast<B2Node*>(contact->GetFixtureA()->GetBody()->GetUserData());
    B2Node * nodeB = static_cast<B2Node*>(contact->GetFixtureB()->GetBody()->GetUserData());
    CCAssert(NULL != nodeA && NULL != nodeB, "the userdata of b2Body is NULL");
    
    // no callback for collision with the worde edge
    if (nodeA->isWorldEdge() || nodeB->isWorldEdge()) {
        return;
    }

    ContactItem item;
    if (nodeA->getRole() <= nodeB->getRole()) {
        item.nodeA = nodeA;
        item.nodeB = nodeB;
    } else {
        item.nodeA = nodeB;
        item.nodeB = nodeA;
    }
    
    // avoid reduplicate recording
    if (m_vContacts.empty()
        || m_vContacts.count(item) == 0) {
        m_vContacts.insert(ContactMap::value_type(item, kContactStateCreated));
    }
}

void QIXB2ContactListener::removeContact(b2Contact *contact) {
    B2Node * nodeA = static_cast<B2Node*>(contact->GetFixtureA()->GetBody()->GetUserData());
    B2Node * nodeB = static_cast<B2Node*>(contact->GetFixtureB()->GetBody()->GetUserData());
    CCAssert(NULL != nodeA && NULL != nodeB, "the userdata of b2Body is NULL");
    
    // no callback for collision with the worde edge
    if (nodeA->isWorldEdge() || nodeB->isWorldEdge()) {
        return;
    }
    
    ContactItem item;
    if (nodeA->getRole() <= nodeB->getRole()) {
        item.nodeA = nodeA;
        item.nodeB = nodeB;
    } else {
        item.nodeA = nodeB;
        item.nodeB = nodeA;
    }
    
    if (m_vContacts.count(item) != 0) {
        // mark to delete
        m_vContacts[item] = kContactStateShouldDelete;

    }
}

void QIXB2ContactListener::BeginContact(b2Contact* contact){
//    CCLOG("BeginContact entered");
    this->addContact(contact);
}

void QIXB2ContactListener::EndContact(b2Contact* contact) {
//    CCLOG("EndContact entered");
    this->removeContact(contact);
}

void QIXB2ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
//    CCLOG("PreSolve entered");
    
    B2Node * nodeA = static_cast<B2Node*>(contact->GetFixtureA()->GetBody()->GetUserData());
    B2Node * nodeB = static_cast<B2Node*>(contact->GetFixtureB()->GetBody()->GetUserData());
    CCAssert(NULL != nodeA && NULL != nodeB, "the userdata of b2Body is NULL");
    
    // deal with sensor
    if ((nodeA->getSensor() && !nodeB->isWorldEdge())
        || (nodeB->getSensor() && !nodeA->isWorldEdge())) {
        contact->SetEnabled(false);
    }
}

void QIXB2ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
//    CCLOG("PostSolve entered");
}

void QIXB2ContactListener::DealWithCollisions() {
//    CCLOG("==========DealWithCollisions entered");
    CCAssert(NULL != m_pDelegate, "Delegate should be set for QIXB2ContactListener");
    
    for (ContactMap::iterator iterator = m_vContacts.begin(); iterator != m_vContacts.end();) {
        B2Node* nodeA = (iterator->first).nodeA;
        B2Node* nodeB = (iterator->first).nodeB;
        // deal with un-processed contacts
        switch (iterator->second) {
            case kContactStateCreated:
                if (nodeA->canResponseToCollision() && nodeB->canResponseToCollision()) {
                    m_pDelegate->onCollisionEvent(nodeA, nodeB);
                    /** ATTETION:
                     If nodeA OR nodeB is deleted in onCollisionEvent, EndContact() will be called in Destroy(b2body).
                     In this case, the state should be kContactStateShouldDelete.
                     As a result, ContactItem can not be erased in EndContact (avoid run iterator)
                     */
                    if (iterator->second == kContactStateCreated) {
                        iterator->second = kContactStateProcessed;
                    }
                }
                iterator++;
                break;
                
            case kContactStateProcessed:
                /* wait to be delete*/
                iterator++;
                break;
                
            case kContactStateShouldDelete:
                m_vContacts.erase(iterator++);
                break;
                
            default:
                break;
        }
    }
//    CCLOG("==========DealWithCollisions ended");
}


void QIXB2ContactListener::registerNotificationMessage() {
    CCNotificationCenter::sharedNotificationCenter()->addObserver(
                                                    this,
                                                    callfuncO_selector(QIXB2ContactListener::receiveNotificationMessage),
                                                    CCNC_MSG_ELEMENT_KILLED,
                                                    NULL);
}

void QIXB2ContactListener::unregisterNotificationMessage() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(
                                                    this,
                                                    CCNC_MSG_ELEMENT_KILLED);
}

void QIXB2ContactListener::receiveNotificationMessage(cocos2d::CCObject *obj) {
    CCLOG("Receive message {%s}", CCNC_MSG_ELEMENT_KILLED);
    
    B2Node* node = static_cast<B2Node*>(obj);
    
    // mark items containing @node to be kContactStateShouldDelete
    for (ContactMap::iterator iterator = m_vContacts.begin(); iterator != m_vContacts.end(); ++iterator) {
        if (node == (iterator->first).nodeA
            || node == (iterator->first).nodeB) {
            iterator->second = kContactStateShouldDelete;
        }
    }
}