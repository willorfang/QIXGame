//
//  Box2DWorld.h
//  QIXGame
//
//  Created by Wei Fang on 3/12/14.
//
//

#ifndef __QIXGame_m_pBox2DWorld__
#define __QIXGame_m_pBox2DWorld__

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "QIXMacros.h"
#include "GLES-Render.h"
#include "QIXB2ContactListener.h"
#include "QIXHelper.h"

#include <vector>
#include <map>

class B2Node;
class B2Sprite;
class B2Edge;

/**
 @brief Defines an Box2D physical world
 @class Box2DWorld
 
 Defines an Box2D physical world.
 1) update the B2Node state according to its b2Body state,
 2) register the world into game engine main loop
 3) define the callback when there are any contacts in the Box2D world
 */
class Box2DWorld : public cocos2d::CCNode {
public:
    //! create the world instance
    SINGLETON_FUNC(Box2DWorld);
    
    //! set world gravity
    void setWorldGravity(float x, float y);
    //! set boundaries of the world with arbitrary scope
    void createBoundaryFromRect(const cocos2d::CCRect& rect);
    
    //! create and add a body into the world
    bool addSpriteBody(B2Sprite* sprite);
    bool addEdgeBody(B2Edge* node, const QIXPolygon& origin);
    //! delete a body from the world
    bool removeBody(B2Node* node);
    
    //! set physical body size
    static bool setPhysicalBodySize(B2Sprite* sprite, float radius);
    
    //! delegate to deal with contacts
    void setContactListenerDelegate(QIXB2ContactListener* delegate);
    const QIXB2ContactListener* getContactListenerDelegate() const {
        return m_pDelegate;
    }
    
protected:
    //! initialize the world
    virtual bool init();
    //! construct
    Box2DWorld();
    //! destruct
    virtual ~Box2DWorld();
    
    //! update the world in every frame
    virtual void update(float dt);
    
    //! to draw Box2D debug view
    virtual void draw();
    
    //! set boundaries of the world
    void setWorldBoundaries();
    
private:
    //! boundary points
    QIXPolygon m_vBoundaryLines;
    //! defines the edge of the world
    B2Node* m_pEdgeNode;
    //! contact listener
    QIXB2ContactListener* m_pDelegate;
    //! Box2D debug draw
    GLESDebugDraw *m_pDebugDraw;
    //! the box2d world
    b2World* m_pWorld;
    //! the singleton
    static Box2DWorld* m_pInstance;
};

#endif /* defined(__QIXGame_m_pBox2DWorld__) */
