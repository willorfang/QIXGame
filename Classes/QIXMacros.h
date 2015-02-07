//
//  QIXMacros.h
//  QIXGame
//
//  Created by Wei Fang on 5/7/14.
//
//

#ifndef QIXGame_QIXMacros_h
#define QIXGame_QIXMacros_h

/** for Box2DWorld
 */
//! PTM ratio in box2d
#define PTM_RADIO                                       32.0f
//! ratio : b2body-size / sprite-size
#define BODY_SPRITE_AREA_RATIO                          0.8f

//! whether to show box2d's debug view or not
//#define OPEN_BOX2D_DEBUG_VIEW

/** for QIXHelper
 */
//! the label-sprite ratio of a button
#define BUTTON_LABEL_SPRITE_RATIO                       0.4f



//! effect values
#define EFFECT_REPEAT_FOREVER_TIME                      -1

//! panel height scale value
#define PANEL_HEIGHT_SCALE_VALUE                        1.5f

/** for area clear
 */
#define AREA_SIZE_ILLEGEL                               0.0f
#define UNSIGNED_INT_VALUE_ILLEGAL                      -1

//! CCNotificationCenter message
#define CCNC_MSG_INTRO_PANEL_TOUCHED                    "IntroPanelTouched"
#define CCNC_MSG_ELEMENT_KILLED                         "ElementKilled"

/**
 @brief scale type definition of sprite
 */
typedef enum {
    // scaleX = scaleX, scaleY = scaleY
    kScaleExactFit = 0,
    // scaleX = scaleY = MAX(sclaeX, scaleY)
    kScaleNoBorder,
    // scaleX = scaleY = MIN(sclaeX, scaleY)
    kScaleShowAll
} kScaleType;

/**
 @brief type definition of the B2Node object
 */
typedef enum {
    kRoleTypeDefault = 0,
    kRoleTypeWorldEdge,
    kRoleTypeMapArea,
    kRoleTypeStaticEnemy,
    kRoleTypeLineEnemy,
    kRoleTypeTrackEnemy,
    kRoleTypeHoleGrid,
    kRoleTypeWallGrid,
    kRoleTypePlayer
} RoleType;

static const char* g_sRoleName[] = {
    "Default",
    "WorldEdge",
    "MapArea",
    "StaticEnemy",
    "LineEnemy",
    "TrackEnemy",
    "HoleGrid",
    "WallGrid",
    "Player"
};

/**
 define a singleton-initializing function for a specific type
 the two should be defined
 1) m_pInstance : the singleton
 2) init() : the init function
 */
#define SINGLETON_FUNC(__TYPE__)                    \
static __TYPE__* GetThe##__TYPE__()                 \
{                                                   \
    if (NULL != m_pInstance) {                      \
        return m_pInstance;                         \
    } else {                                        \
        m_pInstance = new __TYPE__();               \
        if (m_pInstance && m_pInstance->init())     \
        {                                           \
            m_pInstance->autorelease();             \
            CCLOG(#__TYPE__" is created");          \
            return m_pInstance;                     \
        } else {                                    \
            CC_SAFE_DELETE(m_pInstance);            \
            return NULL;                            \
        }                                           \
    }                                               \
}

#define SINGLETON_RETAIN_FUNC(__TYPE__)             \
static __TYPE__* GetThe##__TYPE__()                 \
{                                                   \
    if (NULL != m_pInstance) {                      \
        return m_pInstance;                         \
    } else {                                        \
        m_pInstance = new __TYPE__();               \
        if (m_pInstance && m_pInstance->init())     \
        {                                           \
            m_pInstance->retain();                  \
            m_pInstance->autorelease();             \
            CCLOG(#__TYPE__" is created");          \
            return m_pInstance;                     \
        } else {                                    \
            CC_SAFE_DELETE(m_pInstance);            \
            return NULL;                            \
        }                                           \
    }                                               \
}

#endif
