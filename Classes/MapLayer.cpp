//
//  MapLayer.cpp
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#include "MapLayer.h"
#include "QIXHelper.h"
#include "GameScene.h"

USING_NS_CC;

//! map_area < grid < enemy
#define LAYOUT_ORDER_SAFETY_AREA        1

#define LAYOUT_ORDER_WALL_GRID          10
#define LAYOUT_ORDER_HOLE_GRID          11

#define LAYOUT_ORDER_STATIC_ENEMY       21
#define LAYOUT_ORDER_LINE_ENEMY         22
#define LAYOUT_ORDER_TRACK_ENEMY        23

bool MapLayer::init()
{
    if (CCLayer::init()) {
        // default setting
        m_uTotalEnemyCount = 0;
        
        // add map elements here
        this->initializeElements();
        
        // register update()
        this->scheduleUpdate();
        
        return true;
    }
    return false;
}

MapLayer::MapLayer() {
    
}

MapLayer::~MapLayer() {
    
}

CCRect MapLayer::getRectArea() {
    return QIXLayout::getMapAreaRect();
}

void MapLayer::addStaticEnemy(const MapElementConfig& config) {
    StaticEnemy* enemy = StaticEnemy::create(config);
    m_pMapArea->addClippingChild(enemy, LAYOUT_ORDER_STATIC_ENEMY);
    m_vEnemyList.push_back(enemy);
    ++m_uTotalEnemyCount;
    CCLOG("StaticEnemy is created");
}

void MapLayer::addLineEnemy(const MapElementConfig& config, LineEnemyDirectionType type) {
    LineEnemy* enemy = LineEnemy::create(config);
    enemy->setDirectionType(type);
    m_pMapArea->addClippingChild(enemy, LAYOUT_ORDER_LINE_ENEMY);
    m_vEnemyList.push_back(enemy);
    ++m_uTotalEnemyCount;
    CCLOG("LineEnemy is created");
}

void MapLayer::addTrackEnemy(const MapElementConfig& config) {
    TrackEnemy* enemy = TrackEnemy::create(config);
    // set track target
    /** the player is created HERE
     */
    enemy->setTarget(Player::GetThePlayer());
    m_pMapArea->addClippingChild(enemy, LAYOUT_ORDER_TRACK_ENEMY);
    m_vEnemyList.push_back(enemy);
    ++m_uTotalEnemyCount;
    CCLOG("TrackEnemy is created");
}

void MapLayer::addHoleGrid(const MapElementConfig& config) {
    HoleGrid* grid = HoleGrid::create(config);
    m_pMapArea->addClippingChild(grid, LAYOUT_ORDER_HOLE_GRID);
    m_vMapElementList.push_back(grid);
    CCLOG("HoleGrid is created");
}

void MapLayer::addWallGrid(const MapElementConfig& config) {
    WallGrid* grid = WallGrid::create(config);
    m_pMapArea->addClippingChild(grid, LAYOUT_ORDER_WALL_GRID);
    m_vMapElementList.push_back(grid);
    CCLOG("WallGrid is created");
}

void MapLayer::addBackground() {
    CCRect rectEdge = this->getRectArea();
    const char* filename = QIXResourceInstance.m_oGlobalConfig.MapBackgroundImage.c_str();
    m_pBackgroundSprite = QIXCocos2dHelper::createSpriteWithRect(filename, rectEdge, kScaleExactFit);
    m_pMapArea->addClippingChild(m_pBackgroundSprite, LAYOUT_ORDER_SAFETY_AREA);
}

void MapLayer::initializeMapArea() {
    m_pMapArea = MapArea::GetTheMapArea();
    CCRect rect = this->getRectArea();
    QIXPolygon edge;
    edge.push_back(ccp(rect.origin.x, rect.origin.y));
    edge.push_back(ccp(rect.origin.x, rect.origin.y+rect.size.height));
    edge.push_back(ccp(rect.origin.x+rect.size.width, rect.origin.y+rect.size.height));
    edge.push_back(ccp(rect.origin.x+rect.size.width, rect.origin.y));
    m_pMapArea->setBoundaryArea(edge);
    this->addChild(m_pMapArea);
}

void MapLayer::initializeElements() {
    // initialize map area
    this->initializeMapArea();
    // add background
    this->addBackground();
    
    // add elements
    for (int i=0; i<QIXResourceInstance.m_oLevelConfig.MapElements.size(); ++i) {
        MapElementConfig& config = QIXResourceInstance.m_oLevelConfig.MapElements[i];
        const char* type = config.Type.c_str();
        if (0 == strcmp(type, g_sRoleName[kRoleTypeStaticEnemy])) {
            /* add info for creating map element*/
            config.Image = QIXResourceInstance.m_oGlobalConfig.StaticEnemyImage;
            config.Size = QIXResourceInstance.m_oGlobalConfig.StaticEnemySize;
            this->addStaticEnemy(config);
        } else if (0 == strcmp(type, g_sLineEnemyRoleName[kLineEnemyDirectionTypeToLeft])
                   || 0 == strcmp(type, g_sLineEnemyRoleName[kLineEnemyDirectionTypeToRight])
                   || 0 == strcmp(type, g_sLineEnemyRoleName[kLineEnemyDirectionTypeToTop])
                   || 0 == strcmp(type, g_sLineEnemyRoleName[kLineEnemyDirectionTypeToBottom])) {
            LineEnemyDirectionType directionType = getLineEnemyDirectionTypeFromName(type);
            config.Image = QIXResourceInstance.m_oGlobalConfig.LineEnemyImage;
            config.Size = QIXResourceInstance.m_oGlobalConfig.LineEnemySize;
            config.Velocity = QIXResourceInstance.m_oGlobalConfig.LineEnemyVelocity;
            this->addLineEnemy(config, directionType);
        } else if (0 == strcmp(type, g_sRoleName[kRoleTypeTrackEnemy])) {
            config.Image = QIXResourceInstance.m_oGlobalConfig.TrackEnemyImage;
            config.Size = QIXResourceInstance.m_oGlobalConfig.TrackEnemySize;
            config.Velocity = QIXResourceInstance.m_oGlobalConfig.TrackEnemyVelocity;
            config.Radius = QIXResourceInstance.m_oGlobalConfig.TrackEnemyRadius;
            this->addTrackEnemy(config);
        } else if (0 == strcmp(type, g_sRoleName[kRoleTypeWallGrid])) {
            config.Image = QIXResourceInstance.m_oGlobalConfig.WallGridImage;
            config.Size = QIXResourceInstance.m_oGlobalConfig.WallGridSize;
            this->addWallGrid(config);
        } else if (0 == strcmp(type, g_sRoleName[kRoleTypeHoleGrid])) {
            config.Image = QIXResourceInstance.m_oGlobalConfig.HoleGridImage;
            config.Size = QIXResourceInstance.m_oGlobalConfig.HoleGridSize;
            this->addHoleGrid(config);
        } else {
            CCLOG("Not defined type: %s", type);
            CCAssert(false, "");
        }
    }
}

void MapLayer::update(float dt) {
    // delete map elements when
    // 1) it is not in map area
    // 2) it is marked to delete
    
    // deal with enemy element
    std::list<B2Sprite*>::iterator iterator = m_vMapElementList.begin();
    for (; iterator != m_vMapElementList.end();) {
        // mark as delete if it is not in map area
        if (false == m_pMapArea->containsPoint((*iterator)->getPosition())) {
            (*iterator)->setWillDelete(true);
        }
        // delete elements which is marked as delete
        if ((*iterator)->getWillDelete()) {
            B2Sprite* sprite = (*iterator);
            CCLOG("Delete an element, type = %s", sprite->getRoleName());
            
            // delete from the list
            m_vMapElementList.erase(iterator++);
            
            // clear from the world
            sprite->beKilled();
        } else {
            iterator++;
        }
    }
    
    // deal with grid element
    iterator = m_vEnemyList.begin();
    for (; iterator != m_vEnemyList.end();) {
        if (false == m_pMapArea->containsPoint((*iterator)->getPosition())) {
            (*iterator)->setWillDelete(true);
        }
        
        if ((*iterator)->getWillDelete()) {
            B2Sprite* sprite = (*iterator);
            CCLOG("Delete an element, type = %s", sprite->getRoleName());
            
            // delete from the list
            m_vEnemyList.erase(iterator++);
            
            // clear from the world
            sprite->beKilled();
            
            // update panel
            GameScene* scene = static_cast<GameScene*>(this->getParent());
            CCAssert(NULL != scene, "");
            scene->getPanelLayer()->setEnemyCount(m_vEnemyList.size());
        } else {
            iterator++;
        }
    }
}

bool MapLayer::isStageClear() {
    return m_vEnemyList.size() == 0 && m_pMapArea->isAreaClearFinished();
}

int MapLayer::getAreaClearRatio() {
    return m_pMapArea->getAreaClearRatio();
}
