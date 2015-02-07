//
//  QIXResource.cpp
//  QIXGame
//
//  Created by Wei Fang on 5/14/14.
//
//

#include "QIXResource.h"
#include "QIXAudioEngine.h"

//! resource file name
#define RESOURCE_FILENAME                       "resource.plist"
#define GLOBAL_CONFIG_FILENAME                  "global_config.plist"
#define LOAD_SCENE_CONFIG_FILENAME              "load_scene.plist"
#define LOGIN_SCENE_CONFIG_FILENAME             "login_scene.plist"
#define END_SCENE_CONFIG_FILENAME               "end_scene.plist"

USING_NS_CC;

/* the instance*/
QIXResource QIXResourceInstance;

QIXResource::QIXResource() {
    CCLOG("QIXResource instance is created");
}

void QIXResource::initialize() {
    this->parseResourceData();
    this->parseGlobalConfigData();
    this->parseLoadSceneConfigData();
    this->parseLoginSceneConfigData();
    this->parseEndSceneConfigData();
}

QIXResource::~QIXResource() {
}

bool QIXResource::parseResourceData() {
    CCLOG("load resource plist file");
    
    // load plist file
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(RESOURCE_FILENAME);
    CCAssert(NULL != dic, "");
    
    // parse plist file
    CCObject* obj;
    CCArray* array;
    
    obj = dic->objectForKey("image");
    CCAssert(NULL != obj, "");
    array = static_cast<CCArray*>(obj);
    for (int i=0; i<array->count(); ++i) {
        const char* str = static_cast<CCString*>(array->objectAtIndex(i))->getCString();
        m_vImages.push_back(str);
    }
    
    obj = dic->objectForKey("sound");
    CCAssert(NULL != obj, "");
    array = static_cast<CCArray*>(obj);
    for (int i=0; i<array->count(); ++i) {
        const char* str = static_cast<CCString*>(array->objectAtIndex(i))->getCString();
        m_vSounds.push_back(str);
    }
    
    return true;
}

bool QIXResource::parseGlobalConfigData() {
    CCLOG("load global config plist file");
    
    // load plist file
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(GLOBAL_CONFIG_FILENAME);
    CCAssert(NULL != dic, "");
    
    /* system setting */
    m_oGlobalConfig.SpriteVelocityMin = dic->valueForKey("SpriteVelocityMin")->floatValue();
    m_oGlobalConfig.GameLevelMax = dic->valueForKey("GameLevelMax")->intValue();
    
    /* scene item setting */
    m_oGlobalConfig.BackgroundMusic = dic->valueForKey("BackgroundMusic")->getCString();
    m_oGlobalConfig.EdgeBackgroundImage = dic->valueForKey("EdgeBackgroundImage")->getCString();
    m_oGlobalConfig.MapBackgroundImage = dic->valueForKey("MapBackgroundImage")->getCString();
    m_oGlobalConfig.PlayerBackgroundImage = dic->valueForKey("PlayerBackgroundImage")->getCString();
    m_oGlobalConfig.GamePassSound = dic->valueForKey("GamePassSound")->getCString();
    m_oGlobalConfig.GameFailSound = dic->valueForKey("GameFailSound")->getCString();
    m_oGlobalConfig.ItemSizeDefault = CCSizeFromString(dic->valueForKey("ItemSizeDefault")->getCString());
    m_oGlobalConfig.ReplayButtonImage = dic->valueForKey("ReplayButtonImage")->getCString();
    m_oGlobalConfig.ReplayButtonSize = CCSizeFromString(dic->valueForKey("ReplayButtonSize")->getCString());
    m_oGlobalConfig.TopPanelImage = dic->valueForKey("TopPanelImage")->getCString();
    m_oGlobalConfig.TopPanelItemImage = dic->valueForKey("TopPanelItemImage")->getCString();
    m_oGlobalConfig.BottomPanelImage = dic->valueForKey("BottomPanelImage")->getCString();
    m_oGlobalConfig.BottomPanelItemImage = dic->valueForKey("BottomPanelItemImage")->getCString();
    m_oGlobalConfig.IntroPanelBackgroundImage = dic->valueForKey("IntroPanelBackgroundImage")->getCString();
    m_oGlobalConfig.LabelFontDefault = dic->valueForKey("LabelFontDefault")->getCString();
    m_oGlobalConfig.ButtonNormalImage = dic->valueForKey("ButtonNormalImage")->getCString();
    m_oGlobalConfig.ButtonSelectedImage = dic->valueForKey("ButtonSelectedImage")->getCString();
    m_oGlobalConfig.ButtonHighlightedImage = dic->valueForKey("ButtonHighlightedImage")->getCString();
    m_oGlobalConfig.ButtonDisabledImage = dic->valueForKey("ButtonDisabledImage")->getCString();
    m_oGlobalConfig.ButtonFontDefault = dic->valueForKey("ButtonFontDefault")->getCString();
    m_oGlobalConfig.TitleFontDefault = dic->valueForKey("TitleFontDefault")->getCString();
    
    /* player setting */
    m_oGlobalConfig.PlayerSafeImage = dic->valueForKey("PlayerSafeImage")->getCString();
    m_oGlobalConfig.PlayerActionImage = dic->valueForKey("PlayerActionImage")->getCString();
    m_oGlobalConfig.PlayerKilledSound = dic->valueForKey("PlayerKilledSound")->getCString();
    m_oGlobalConfig.AreaClearSound = dic->valueForKey("AreaClearSound")->getCString();
    m_oGlobalConfig.PlayerLaunchSound = dic->valueForKey("PlayerLaunchSound")->getCString();
    m_oGlobalConfig.PlayerLaunchArrowImage = dic->valueForKey("PlayerLaunchArrowImage")->getCString();
    m_oGlobalConfig.PlayerLaunchBanImage = dic->valueForKey("PlayerLaunchBanImage")->getCString();
    m_oGlobalConfig.PlayerLaunchBanSound = dic->valueForKey("PlayerLaunchBanSound")->getCString();
    m_oGlobalConfig.PlayerStepShowboxImage = dic->valueForKey("PlayerStepShowboxImage")->getCString();
    m_oGlobalConfig.PlayerSize = CCSizeFromString(dic->valueForKey("PlayerSize")->getCString());
    m_oGlobalConfig.PlayerLaunchVelocityMin = dic->valueForKey("PlayerLaunchVelocityMin")->floatValue();
    m_oGlobalConfig.PlayerLaunchVelocityMax = dic->valueForKey("PlayerLaunchVelocityMax")->floatValue();
    m_oGlobalConfig.SpriteVelocityControlFactor = dic->valueForKey("SpriteVelocityControlFactor")->floatValue();
    m_oGlobalConfig.PlayerLaunchCountMax = dic->valueForKey("PlayerLaunchCountMax")->intValue();
    m_oGlobalConfig.PlayerPlayTimeMax = dic->valueForKey("PlayerPlayTimeMax")->intValue();
    m_oGlobalConfig.PlayerMissCountMax = dic->valueForKey("PlayerMissCountMax")->intValue();
    m_oGlobalConfig.AreaClearRatioMax = dic->valueForKey("AreaClearRatioMax")->intValue();
    m_oGlobalConfig.PlayerClearPathLength = dic->valueForKey("PlayerClearPathLength")->intValue();
    m_oGlobalConfig.PlayerArrowLengthDefault = dic->valueForKey("PlayerArrowLengthDefault")->floatValue();
    m_oGlobalConfig.PlayerArrowLengthMax = dic->valueForKey("PlayerArrowLengthMax")->floatValue();
    m_oGlobalConfig.PlayerTrackPointsCountDefault = dic->valueForKey("PlayerTrackPointsCountDefault")->intValue();
    
    /* enemy setting */
    m_oGlobalConfig.LineEnemyImage = dic->valueForKey("LineEnemyImage")->getCString();
    m_oGlobalConfig.LineEnemySize = CCSizeFromString(dic->valueForKey("LineEnemySize")->getCString());
    m_oGlobalConfig.LineEnemyVelocity = dic->valueForKey("LineEnemyVelocity")->floatValue();
    m_oGlobalConfig.StaticEnemyImage = dic->valueForKey("StaticEnemyImage")->getCString();
    m_oGlobalConfig.StaticEnemySize = CCSizeFromString(dic->valueForKey("StaticEnemySize")->getCString());
    m_oGlobalConfig.TrackEnemyImage = dic->valueForKey("TrackEnemyImage")->getCString();
    m_oGlobalConfig.TrackEnemySize = CCSizeFromString(dic->valueForKey("TrackEnemySize")->getCString());
    m_oGlobalConfig.TrackEnemyVelocity = dic->valueForKey("TrackEnemyVelocity")->floatValue();
    m_oGlobalConfig.TrackEnemyRadius = dic->valueForKey("TrackEnemyRadius")->floatValue();
    m_oGlobalConfig.WallGridImage = dic->valueForKey("WallGridImage")->getCString();
    m_oGlobalConfig.WallGridSize = CCSizeFromString(dic->valueForKey("WallGridSize")->getCString());
    m_oGlobalConfig.HoleGridImage = dic->valueForKey("HoleGridImage")->getCString();
    m_oGlobalConfig.HoleGridSize = CCSizeFromString(dic->valueForKey("HoleGridSize")->getCString());
    return true;
}

bool QIXResource::parseLoadSceneConfigData() {
    CCLOG("load GameLoadScene config plist file");
    
    // load plist file
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(LOAD_SCENE_CONFIG_FILENAME);
    CCAssert(NULL != dic, "");
    
    // parse plist file
    m_oLoadSceneConfig.BackgroundMusic = dic->valueForKey("BackgroundMusic")->getCString();
    m_oLoadSceneConfig.BackgroundImage = dic->valueForKey("BackgroundImage")->getCString();
    
    CCObject* obj;
    CCDictionary* dicItem;
    
    obj = dic->objectForKey("TitleLabel");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoadSceneConfig.TitleLabel.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoadSceneConfig.TitleLabel.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oLoadSceneConfig.TitleLabel.Font = dicItem->valueForKey("Font")->getCString();
    
    obj = dic->objectForKey("LoadingLabel");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoadSceneConfig.LoadingLabel.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoadSceneConfig.LoadingLabel.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oLoadSceneConfig.LoadingLabel.Font = dicItem->valueForKey("Font")->getCString();
    
    obj = dic->objectForKey("ProgressBar");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoadSceneConfig.ProgressBar.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoadSceneConfig.ProgressBar.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oLoadSceneConfig.ProgressBar.Image = dicItem->valueForKey("Image")->getCString();
    
    obj = dic->objectForKey("ProgressBarBackground");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoadSceneConfig.ProgressBarBackground.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoadSceneConfig.ProgressBarBackground.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oLoadSceneConfig.ProgressBarBackground.Image = dicItem->valueForKey("Image")->getCString();
    
    return true;
}

bool QIXResource::parseLoginSceneConfigData() {
    CCLOG("load GameLoginScene config plist file");
    // load plist file
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(LOGIN_SCENE_CONFIG_FILENAME);
    CCAssert(NULL != dic, "");
    
    // parse plist file
    m_oLoginSceneConfig.BackgroundMusic = dic->valueForKey("BackgroundMusic")->getCString();
    m_oLoginSceneConfig.BackgroundImage = dic->valueForKey("BackgroundImage")->getCString();
    
    CCObject* obj;
    CCDictionary* dicItem;
    
    obj = dic->objectForKey("TitleLabel");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoginSceneConfig.TitleLabel.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoginSceneConfig.TitleLabel.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oLoginSceneConfig.TitleLabel.Font = dicItem->valueForKey("Font")->getCString();
    
    obj = dic->objectForKey("PlayButton");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLoginSceneConfig.PlayButton.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oLoginSceneConfig.PlayButton.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    
    return true;
}

bool QIXResource::parseEndSceneConfigData() {
    CCLOG("load GameEndScene config plist file");
    
    // load plist file
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(END_SCENE_CONFIG_FILENAME);
    CCAssert(NULL != dic, "");
    
    // parse plist file
    m_oEndSceneConfig.BackgroundMusic = dic->valueForKey("BackgroundMusic")->getCString();
    m_oEndSceneConfig.BackgroundImage = dic->valueForKey("BackgroundImage")->getCString();
    
    CCObject* obj;
    CCDictionary* dicItem;
    
    obj = dic->objectForKey("GameStatusLabel");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oEndSceneConfig.GameStatusLabel.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oEndSceneConfig.GameStatusLabel.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oEndSceneConfig.GameStatusLabel.Font = dicItem->valueForKey("Font")->getCString();
    
    obj = dic->objectForKey("ScoreLabel");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oEndSceneConfig.ScoreLabel.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oEndSceneConfig.ScoreLabel.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oEndSceneConfig.ScoreLabel.Font = dicItem->valueForKey("Font")->getCString();
    
    obj = dic->objectForKey("ScoreData");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oEndSceneConfig.ScoreData.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oEndSceneConfig.ScoreData.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    m_oEndSceneConfig.ScoreData.Font = dicItem->valueForKey("Font")->getCString();
    m_oEndSceneConfig.ScoreData.Image = dicItem->valueForKey("Image")->getCString();
    
    obj = dic->objectForKey("ReplayButton");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oEndSceneConfig.ReplayButton.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oEndSceneConfig.ReplayButton.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    
    obj = dic->objectForKey("NextButton");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oEndSceneConfig.NextButton.Size = CCSizeFromString(dicItem->valueForKey("Size")->getCString());
    m_oEndSceneConfig.NextButton.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    return true;
}

const char* QIXResource::getFilenameFromLevel(int order) {
    const char* filename = CCString::createWithFormat("level/%d.plist", order)->getCString();
    return filename;
}

LevelConfig& QIXResource::parseInfoForNewLevel(int order) {
    CCLOG("load level %d plist file", order);
    
    // clear cache
    m_oLevelConfig.MapElements.clear();
    
    // load plist file
    const char* filename = QIXResource::getFilenameFromLevel(order);
    CCDictionary* dic = CCDictionary::createWithContentsOfFile(filename);
    CCAssert(NULL != dic, "");
    
    // parse plist file
    CCObject* obj;
    CCDictionary* dicItem;
    
    /* player */
    obj = dic->objectForKey("Player");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    m_oLevelConfig.Player.Position = CCPointFromString(dicItem->valueForKey("Position")->getCString());
    
    /* pass condition */
    obj = dic->objectForKey("PassCondition");
    CCAssert(NULL != obj, "");
    dicItem = static_cast<CCDictionary*>(obj);
    // scope limit
    int value;
    value = dicItem->valueForKey("MissCountMax")->intValue();
    if (value <= 0 || value > m_oGlobalConfig.PlayerMissCountMax) {
        value = m_oGlobalConfig.PlayerMissCountMax;
    }
    m_oLevelConfig.PassCondition.MissCountMax = value;
    //
    value = dicItem->valueForKey("LaunchCountMax")->intValue();
    if (value <= 0 || value > m_oGlobalConfig.PlayerLaunchCountMax) {
        value = m_oGlobalConfig.PlayerLaunchCountMax;
    }
    m_oLevelConfig.PassCondition.LaunchCountMax = value;
    //
    value = dicItem->valueForKey("PlayTimeMax")->intValue();
    if (value <= 0 || value > m_oGlobalConfig.PlayerPlayTimeMax) {
        value = m_oGlobalConfig.PlayerPlayTimeMax;
    }
    m_oLevelConfig.PassCondition.PlayTimeMax = value;
    //
    value = dicItem->valueForKey("AreaClearRatio")->intValue();
    if (value <= 0 || value > m_oGlobalConfig.AreaClearRatioMax) {
        value = m_oGlobalConfig.AreaClearRatioMax;
    }
    m_oLevelConfig.PassCondition.AreaClearRatio = value;

    /* Enemy */
    obj = dic->objectForKey("Enemy");
    CCAssert(NULL != obj, "");
    CCArray* array = static_cast<CCArray*>(obj);
    CCObject* object;
    CCARRAY_FOREACH(array, object) {
        CCDictionary *dic = static_cast<CCDictionary*>(object);
        MapElementConfig config;
        config.Type = dic->valueForKey("Type")->getCString();
        config.Position = CCPointFromString(dic->valueForKey("Position")->getCString());
        m_oLevelConfig.MapElements.push_back(config);
    }
    
    return m_oLevelConfig;
}
