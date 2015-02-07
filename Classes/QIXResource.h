//
//  QIXResource.h
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#ifndef __QIXGame__QIXResource__
#define __QIXGame__QIXResource__

#include "cocos2d.h"
#include "QIXMacros.h"

#include <vector>
#include <string>

//! config information for load scene
typedef struct {
    cocos2d::CCSize Size;
    cocos2d::CCPoint Position;
    std::string Font;
} LabelConfig;

typedef struct {
    cocos2d::CCSize Size;
    cocos2d::CCPoint Position;
    std::string Image;
    std::string Font;
} ButtonConfig;

typedef struct {
    cocos2d::CCSize Size;
    cocos2d::CCPoint Position;
    std::string Image;
} SpriteConfig;

typedef struct {
    int MissCountMax;
    int LaunchCountMax;
    int PlayTimeMax;
    int AreaClearRatio;
} PassConditionConfig;

typedef struct {
    std::string BackgroundImage;
    std::string BackgroundMusic;
    LabelConfig TitleLabel;
    LabelConfig LoadingLabel;
    SpriteConfig ProgressBar;
    SpriteConfig ProgressBarBackground;
} LoadSceneConfig;

typedef struct {
    std::string BackgroundImage;
    std::string BackgroundMusic;
    LabelConfig TitleLabel;
    ButtonConfig PlayButton;
} LoginSceneConfig;

typedef struct {
    std::string BackgroundImage;
    std::string BackgroundMusic;
    LabelConfig GameStatusLabel;
    LabelConfig ScoreLabel;
    ButtonConfig ScoreData;
    ButtonConfig ReplayButton;
    ButtonConfig NextButton;
} EndSceneConfig;

//! config information for global
typedef struct {
    /* system setting */
    float SpriteVelocityMin;
    int GameLevelMax;
    
    /* scene item setting */
    std::string BackgroundMusic;
    std::string EdgeBackgroundImage;
    std::string MapBackgroundImage;
    std::string PlayerBackgroundImage;
    std::string GamePassSound;
    std::string GameFailSound;
    cocos2d::CCSize ItemSizeDefault;
    std::string ReplayButtonImage;
    cocos2d::CCSize ReplayButtonSize;
    std::string TopPanelImage;
    std::string TopPanelItemImage;
    std::string BottomPanelImage;
    std::string BottomPanelItemImage;
    std::string IntroPanelBackgroundImage;
    std::string LabelFontDefault;
    std::string ButtonNormalImage;
    std::string ButtonSelectedImage;
    std::string ButtonHighlightedImage;
    std::string ButtonDisabledImage;
    std::string ButtonFontDefault;
    std::string TitleFontDefault;
    
    /* player setting */
    std::string PlayerSafeImage;
    std::string PlayerActionImage;
    std::string PlayerKilledSound;
    std::string AreaClearSound;
    std::string PlayerLaunchSound;
    std::string PlayerLaunchArrowImage;
    std::string PlayerLaunchBanImage;
    std::string PlayerLaunchBanSound;
    std::string PlayerStepShowboxImage;
    cocos2d::CCSize PlayerSize;
    float PlayerLaunchVelocityMin;
    float PlayerLaunchVelocityMax;
    float SpriteVelocityControlFactor;
    int PlayerLaunchCountMax;
    int PlayerPlayTimeMax;
    int PlayerMissCountMax;
    int AreaClearRatioMax;
    int PlayerClearPathLength;
    float PlayerArrowLengthDefault;
    float PlayerArrowLengthMax;
    int PlayerTrackPointsCountDefault;
    
    
    /* enemy setting */
    std::string LineEnemyImage;
    cocos2d::CCSize LineEnemySize;
    float LineEnemyVelocity;
    std::string StaticEnemyImage;
    cocos2d::CCSize StaticEnemySize;
    std::string TrackEnemyImage;
    cocos2d::CCSize TrackEnemySize;
    float TrackEnemyVelocity;
    float TrackEnemyRadius;
    std::string WallGridImage;
    cocos2d::CCSize WallGridSize;
    std::string HoleGridImage;
    cocos2d::CCSize HoleGridSize;
} GlobalConfig;

//! config information for map elements
typedef struct {
    std::string Type;
    std::string Image;
    cocos2d::CCSize Size;
    cocos2d::CCPoint Position;
    float Radius;
    float Velocity;
} MapElementConfig;

typedef std::vector<MapElementConfig> MapElementConfigVector;

typedef struct {
    cocos2d::CCPoint Position;
} PlayerConfig;

typedef struct {
    MapElementConfigVector MapElements;
    PassConditionConfig PassCondition;
    PlayerConfig Player;
} LevelConfig;

typedef std::vector<MapElementConfig> MapElementConfigVector;


/**
 @brief Control all the resource data
 @class QIXResource
 
 Control all the resource data
 */
class QIXResource : public cocos2d::CCObject {
public:
    //! parse level config
    LevelConfig& parseInfoForNewLevel(int order);
    
    //! get data from plist file
    bool parseResourceData();
    bool parseGlobalConfigData();
    bool parseLoadSceneConfigData();
    bool parseLoginSceneConfigData();
    bool parseEndSceneConfigData();
    
public:
    //! construct
    QIXResource();
    //! destruct
    virtual ~QIXResource();
    //! initialize
    void initialize();
    
private:
    //! get level resource filename from level order
    const char* getFilenameFromLevel(int order);
    
public:
    //! all sounds
    std::vector<std::string> m_vSounds;
    //! all Images
    std::vector<std::string> m_vImages;
    //! all animation plists
    // TODO
    
    //! map elements config for a level
    LevelConfig m_oLevelConfig;
    //! global config
    GlobalConfig m_oGlobalConfig;
    //! load scene config
    LoadSceneConfig m_oLoadSceneConfig;
    //! login scene config
    LoginSceneConfig m_oLoginSceneConfig;
    //! end scene config
    EndSceneConfig m_oEndSceneConfig;
};

/**
 for global access
 */
extern QIXResource QIXResourceInstance;

#endif /* defined(__QIXGame__QIXResource__) */
