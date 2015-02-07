//
//  MapLayer.h
//  QIXGame
//
//  Created by Wei Fang on 4/3/14.
//
//

#ifndef __QIXGame__MapLayer__
#define __QIXGame__MapLayer__

#include "cocos2d.h"
#include "QIXMacros.h"
#include "LineEnemy.h"
#include "StaticEnemy.h"
#include "TrackEnemy.h"
#include "WallGrid.h"
#include "HoleGrid.h"
#include "Player.h"
#include "MapArea.h"


/**
 @brief Defines the map layer
 @class MapLayer
 
 Defines the map layer, which includes all map elements in a game scene,
 such as enemy, grid, map area, background
 1) includes all map elements in a game scene
 2) delete map elements in real time
 */
class MapLayer : public cocos2d::CCLayer
{
public:
    //! create an instance
    CREATE_FUNC(MapLayer);
    
    //! whether stage clear or not
    bool isStageClear();

    //! to update game status
    int getAreaClearRatio();
    unsigned int getKilledEnemyCount() const {
        return m_uTotalEnemyCount - m_vEnemyList.size();
    }
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_uTotalEnemyCount, TotalEnemyCount);
    CC_SYNTHESIZE_READONLY(MapArea*, m_pMapArea, MapArea);
    
protected:
    //! initialize
    virtual bool init();
    //! construct
    MapLayer();
    //! destruct
    virtual ~MapLayer();
    
    //! add a map element into the layer
    void addStaticEnemy(const MapElementConfig& config);
    void addLineEnemy(const MapElementConfig& config, LineEnemyDirectionType type);
    void addTrackEnemy(const MapElementConfig& config);
    void addHoleGrid(const MapElementConfig& config);
    void addWallGrid(const MapElementConfig& config);
    void addBackground();
    //! initialize elements
    void initializeElements();
    //! get map rect area
    static cocos2d::CCRect getRectArea();
    //! set map area
    void initializeMapArea();
    
    //! delete all map elements which are not in map area
    virtual void update(float dt);

private:
    //! map background sprite
    cocos2d::CCSprite* m_pBackgroundSprite;
    //! map element list
    std::list<B2Sprite*> m_vEnemyList;
    std::list<B2Sprite*> m_vMapElementList;
};


#endif /* defined(__QIXGame__MapLayer__) */
