//
//  Box2DWorld.cpp
//  QIXGame
//
//  Created by Wei Fang on 3/12/14.
//
//

#include "Box2DWorld.h"
#include "B2Node.h"
#include "B2Edge.h"
#include "B2Sprite.h"
#include "QIXB2ContactListener.h"

USING_NS_CC;

typedef enum {
    kFilterDefault = 0x0001,
    kFilterEnemyBits = 0x0002,
    kFilterPlayerBits = 0x0004,
    kFilterGridBits = 0x0008,
    kFilterMapAreaBits = 0x0010
} FilterBits;

Box2DWorld* Box2DWorld::m_pInstance = NULL;

void Box2DWorld::setWorldGravity(float x, float y) {
    CCAssert(NULL != m_pWorld, "b2World should be initialized first");
    b2Vec2 gravity = b2Vec2(x, y);
    m_pWorld->SetGravity(gravity);
}

void Box2DWorld::createBoundaryFromRect(const cocos2d::CCRect& rect) {
    // initialize boundary points clockwisely
    m_vBoundaryLines.clear();
    // bottom
    m_vBoundaryLines.push_back(ccp(rect.origin.x, rect.origin.y));
    // left
    m_vBoundaryLines.push_back(ccp(rect.origin.x, rect.origin.y + rect.size.height));
    // top
    m_vBoundaryLines.push_back(ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height));
    // right
    m_vBoundaryLines.push_back(ccp(rect.origin.x + rect.size.width, rect.origin.y));
    this->setWorldBoundaries();
}

void Box2DWorld::setWorldBoundaries() {
    CCAssert(NULL != m_pWorld && NULL != m_pEdgeNode && m_vBoundaryLines.size() > 0, "Wrong state for set world boundaries");

    // clear if it is set before
    if (NULL != m_pEdgeNode->getB2Body()) {
        m_pWorld->DestroyBody(m_pEdgeNode->getB2Body());
    }
    
    // create b2Body for edge definitions
    b2BodyDef edgeBodyDef;
    edgeBodyDef.position.Set(0, 0);
    edgeBodyDef.type=b2_staticBody;
    edgeBodyDef.userData = m_pEdgeNode;
    b2Body *body = m_pWorld->CreateBody(&edgeBodyDef);
    m_pEdgeNode->setB2Body(body);
        
    // create fixture for edge definitions
    b2EdgeShape edgeShape;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &edgeShape;
    fixtureDef.density = 0;
    // all the collisions with the boundary will be resolved
    fixtureDef.filter.categoryBits = kFilterDefault;
    
    // define shape
    int count = m_vBoundaryLines.size();
    for (int i=0; i<count; i++) {
        int j = (i+1)%count;
        edgeShape.Set(b2Vec2(m_vBoundaryLines[i].x/PTM_RADIO, m_vBoundaryLines[i].y/PTM_RADIO),
                      b2Vec2(m_vBoundaryLines[j].x/PTM_RADIO, m_vBoundaryLines[j].y/PTM_RADIO));
        body->CreateFixture(&fixtureDef);
    }
}

bool Box2DWorld::init() {
    if (!CCNode::init()) {
        return false;
    }
    
    // default setting
    m_pDelegate = NULL;
    
    // create the default Box2D world
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    m_pWorld = new b2World(gravity);
    m_pWorld->SetAllowSleeping(true);
    m_pWorld->SetContinuousPhysics(true);
    
    // initialize debug draw
    m_pDebugDraw = new GLESDebugDraw(PTM_RADIO);
    m_pWorld->SetDebugDraw(m_pDebugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
//    flags += b2Draw::e_centerOfMassBit;
    m_pDebugDraw->SetFlags(flags);
    
    // set default boundaries
    m_pEdgeNode = B2Node::create();
    m_pEdgeNode->setRole(kRoleTypeWorldEdge);
    m_pEdgeNode->setBodyType(b2_staticBody);
    m_pEdgeNode->retain();
    
    // add into game main loop
    this->scheduleUpdate();
    return true;
}

Box2DWorld::Box2DWorld() {
    
}

Box2DWorld::~Box2DWorld() {
    CC_SAFE_RELEASE_NULL(m_pEdgeNode);
    CC_SAFE_DELETE(m_pWorld);
    CC_SAFE_DELETE(m_pDebugDraw);
    CC_SAFE_RELEASE_NULL(m_pDelegate);
    // avoid dangling pointer
    m_pInstance = NULL;
}

bool Box2DWorld::addSpriteBody(B2Sprite *sprite) {
    // create a ball b2Body
    b2BodyDef ballBodyDef;
    // set body type
    ballBodyDef.type = sprite->getBodyType();
    // set the initial position
    CCPoint pos = sprite->getPosition();
    ballBodyDef.position.Set(pos.x/PTM_RADIO, pos.y/PTM_RADIO);
    // set damping
    ballBodyDef.linearDamping = sprite->getPhysicalParameter().linerDamping;
    ballBodyDef.angularDamping = sprite->getPhysicalParameter().angularDamping;
    // set velocity
    ballBodyDef.linearVelocity = b2Vec2(sprite->getVelocity().x/PTM_RADIO, sprite->getVelocity().y/PTM_RADIO);
    
    // establish the relationship between b2Body and sprite
    ballBodyDef.userData = sprite;
    b2Body* body = m_pWorld->CreateBody(&ballBodyDef);
    sprite->setB2Body(body);
    
    // create shape
    b2CircleShape shape;
    float radius = MIN(sprite->getContentSize().width, sprite->getContentSize().height) / 2;
    shape.m_radius = radius * BODY_SPRITE_AREA_RATIO / PTM_RADIO;
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &shape;
    ballShapeDef.density = sprite->getPhysicalParameter().density;
    ballShapeDef.friction = sprite->getPhysicalParameter().friction;
    ballShapeDef.restitution = sprite->getPhysicalParameter().restitution;
    
    // config collision filter
    // every element of the same group will not collide, while collide of different groups
    switch (sprite->getRole()) {
        case kRoleTypeLineEnemy:
        case kRoleTypeStaticEnemy:
        case kRoleTypeTrackEnemy:
            ballShapeDef.filter.categoryBits = kFilterEnemyBits;
            ballShapeDef.filter.maskBits = kFilterPlayerBits | kFilterGridBits | kFilterDefault | kFilterMapAreaBits;
            break;
        case kRoleTypePlayer:
            ballShapeDef.filter.categoryBits = kFilterPlayerBits;
            ballShapeDef.filter.maskBits = kFilterEnemyBits | kFilterGridBits | kFilterDefault;
            break;
        case kRoleTypeHoleGrid:
        case kRoleTypeWallGrid:
            ballShapeDef.filter.categoryBits = kFilterGridBits;
            ballShapeDef.filter.maskBits = kFilterPlayerBits | kFilterEnemyBits | kFilterDefault | kFilterMapAreaBits;
            break;
        default:
            break;
    }
    
    body->CreateFixture(&ballShapeDef);
    return true;
}

bool Box2DWorld::removeBody(B2Node* node) {
    if (NULL != node) {
        if (NULL != node->getB2Body()) {
            m_pWorld->DestroyBody(node->getB2Body());
            node->setB2Body(NULL);
        }
        return true;
    }
    return false;
}

bool Box2DWorld::setPhysicalBodySize(B2Sprite* sprite, float radius) {
    CCAssert(NULL != sprite, "");
    
    b2Body* physicalBody = sprite->getB2Body();
    if (NULL == physicalBody) {
        return false;
    }
    
    // set shape radius
    b2Shape* shape = physicalBody->GetFixtureList()->GetShape();
    shape->m_radius = radius * BODY_SPRITE_AREA_RATIO;
    return true;
}

bool Box2DWorld::addEdgeBody(B2Edge* node, const QIXPolygon& origin) {
    CCAssert(NULL != m_pWorld && origin.size() >= 2, "Wrong parameters for map area points");
    
    // create a b2Body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0.0f, 0.0f);
    // establish the relationship between b2Body and node
    bodyDef.userData = node;
    b2Body* body = m_pWorld->CreateBody(&bodyDef);
    node->setB2Body(body);
    
    // create fixture for edge definitions
    b2EdgeShape edgeShape;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &edgeShape;
    fixtureDef.density = 0;
    fixtureDef.filter.categoryBits = kFilterMapAreaBits;
    // define shape
    int count = origin.size();
    for (int i=0; i<count; i++) {
        int j = (i+1)%count;
        edgeShape.Set(b2Vec2(origin[i].x/PTM_RADIO, origin[i].y/PTM_RADIO),
                      b2Vec2(origin[j].x/PTM_RADIO, origin[j].y/PTM_RADIO));
        body->CreateFixture(&fixtureDef);
    }
    
    return true;
}

void Box2DWorld::update(float dt) {
    // tranverse all the bodies in the world
    for (b2Body* b=m_pWorld->GetBodyList(); b!=NULL; b=b->GetNext()) {
        if (b && b->GetUserData()) {
            B2Node* node = static_cast<B2Node*>(b->GetUserData());
            CCAssert(NULL != node, "");
            B2Sprite* sprite = dynamic_cast<B2Sprite*>(node);
            if (NULL == sprite) {
                continue;
            }
            
            b2Vec2 pos = b->GetPosition();
            // update the related sprite status
            sprite->setPosition(ccp(pos.x*PTM_RADIO, pos.y*PTM_RADIO));
            if (!sprite->getFixedRotation()) {
                float rotation = -1 * CC_RADIANS_TO_DEGREES(b->GetAngle());
                sprite->setRotation(rotation);
            } else {
                sprite->setRotation(0.0f);
            }
        }
    }
    // update the Box2D world
    m_pWorld->Step(dt, 8, 8);
    // deal with all the collisions
    if (NULL != m_pDelegate) {
        m_pDelegate->DealWithCollisions();
    }
}

void Box2DWorld::draw() {
    CCNode::draw();
#ifdef OPEN_BOX2D_DEBUG_VIEW
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);
    kmGLPushMatrix();
    m_pWorld->DrawDebugData();
    kmGLPopMatrix();
#endif
}

void Box2DWorld::setContactListenerDelegate(QIXB2ContactListener* delegate) {
    CCAssert(NULL != m_pWorld, "b2World should be initialized first");
    delegate->retain();
    if (NULL != m_pDelegate) {
        m_pDelegate->release();
    }
    m_pDelegate = delegate;
    // register contact callbacks
    m_pWorld->SetContactListener(m_pDelegate);
}
