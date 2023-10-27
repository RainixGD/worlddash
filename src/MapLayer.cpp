#include "MapLayer.h"
#include "./Button.h"
#include "./DataManager.h"
#include "./MapUiLayer.h"
#include "./MapUpdateAlert.h"
#include "./FrameAnimatedSprite.h"
#include "./MapDecoContainer.h"
#include <random>

using namespace geode::prelude;


bool MapLayer::init() {
	if (!CCLayer::init()) {
		return false;
	}
	GameSoundManager::sharedManager()->playBackgroundMusic("mapSong.mp3"_spr, 1, 0);

	auto size = CCDirector::sharedDirector()->getWinSize();

	map = CCSprite::create("map.png"_spr);
	map->setPosition(size / 2);
	map->setScale(1.5);
	this->addChild(map);

	levelsMenu = CCMenu::create();
	levelsMenu->setPosition(0, 0);
	map->addChild(levelsMenu);

	this->minScroll = ccp(-map->getContentSize().width * map->getScale() / 2 + size.width / 2, -map->getContentSize().height * map->getScale() / 2 + size.height / 2);
	this->maxScroll = ccp(map->getContentSize().width * map->getScale() / 2 - size.width / 2, map->getContentSize().height * map->getScale() / 2 - size.height / 2);

	this->setPosition(500, 600);//map start position

	mapDecoContainer = MapDecoContainer::create();
	mapDecoContainer->setPosition(0, 0);
	map->addChild(mapDecoContainer);
	
	setTouchEnabled(true);
	setTouchMode(cocos2d::kCCTouchesOneByOne);

	return true;
}

void MapLayer::updateMap(json::Array newLevels) {
	clearMap();

	mapLevels = newLevels;

	for (json::Value lvlJson : mapLevels) {
        if (lvlJson["isLevel"].as_bool()){
			auto level = LevelObj::create(lvlJson, [this]() {this->updateMapAndCoins(); });
			level->setTag(levelTag); //real level
			level->setVisible(!isAddLevelMode);
			levelsMenu->addChild(level);
		}
		else {
			auto addBtnSprite = CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png");
			auto addBtn = Button::create(addBtnSprite, this, menu_selector(MapLayer::onPlusBtnClicked)); // will add callback
			addBtn->setTag(plusTag); //add level btn
			addBtn->setPosition({ float(lvlJson["x"].as_int()), float(lvlJson["y"].as_int()) });
			levelsMenu->addChild(addBtn);
			addBtn->setVisible(isAddLevelMode);
		}
    }
}

void MapLayer::makeUpdateMapReq() {
	auto alert = MapUpdateAlert::create(this);
	alert->m_scene = this->getParent();
	alert->show();
}

void MapLayer::updateMapAndCoins() {
	mapUiLayer->updateCoins();
	makeUpdateMapReq();
}

MapLayer::MapLayer() : isDragging(false), lastTouchPos(cocos2d::CCPointZero) {}

MapLayer* MapLayer::create(MapUiLayer* mapUiLayer) {
	auto ret = new MapLayer();
	ret->mapUiLayer = mapUiLayer;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool MapLayer::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
	cocos2d::CCPoint touchPos = touch->getLocation();
	lastTouchPos = touchPos;
	isDragging = true;
	return true;
}

void MapLayer::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
	if (isDragging) {
		cocos2d::CCPoint touchPos = touch->getLocation();
		cocos2d::CCPoint delta = touchPos - lastTouchPos;
		this->setPosition(this->getPosition() + delta);
		lastTouchPos = touchPos;

		if (this->getPositionX() < this->minScroll.x)
				this->setPositionX(minScroll.x);

		if (this->getPositionY() < this->minScroll.y)
				this->setPositionY(minScroll.y);

		if (this->getPositionX() > this->maxScroll.x)
				this->setPositionX(maxScroll.x);

		if (this->getPositionY() > this->maxScroll.y)
				this->setPositionY(maxScroll.y);

		if (this->getPositionX() <= -300 && this->getPositionY() <= -250) {
			auto oldColor = this->mapUiLayer->snow->getStartColor();
			this->mapUiLayer->snow->setStartColor({ oldColor.r, oldColor.g, oldColor.b, 1 });
			this->mapUiLayer->snow->setEndColor({ oldColor.r, oldColor.g, oldColor.b, 1 });
			this->mapUiLayer->snow->setEndColorVar({ 0, 0, 0, 0.3 });
		}
		else {
			auto oldColor = this->mapUiLayer->snow->getStartColor();
			this->mapUiLayer->snow->setStartColor({ oldColor.r, oldColor.g, oldColor.b, 0 });
			this->mapUiLayer->snow->setEndColor({ oldColor.r, oldColor.g, oldColor.b, 0 });
			this->mapUiLayer->snow->setEndColorVar({ 0, 0, 0, 0 });
		}
	}
}

void MapLayer::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
	isDragging = false;
}
/*
int MapLayer::availableMapPoints[235][2] = {
{581, 133},
{900, 152},
{994, 171},
{450, 171},
{544, 190},
{488, 190},
{975, 208},
{919, 208},
{638, 208},
{881, 227},
{581, 227},
{394, 227},
{544, 246},
{469, 246},
{356, 246},
{825, 265},
{750, 265},
{694, 265},
{619, 265},
{431, 265},
{394, 265},
{319, 265},
{1031, 283},
{506, 283},
{469, 283},
{1256, 302},
{788, 302},
{563, 302},
{300, 302},
{750, 321},
{656, 321},
{1294, 340},
{881, 340},
{600, 340},
{450, 340},
{938, 358},
{750, 358},
{506, 358},
{881, 377},
{825, 377},
{788, 377},
{656, 377},
{225, 377},
{600, 396},
{1050, 415},
{938, 415},
{881, 415},
{844, 415},
{525, 415},
{694, 433},
{319, 433},
{1163, 452},
{1013, 452},
{956, 452},
{656, 452},
{431, 452},
{375, 452},
{225, 452},
{169, 452},
{94, 452},
{806, 471},
{338, 471},
{1238, 490},
{1106, 490},
{975, 490},
{919, 490},
{694, 490},
{394, 490},
{1144, 508},
{1069, 508},
{1013, 508},
{825, 508},
{638, 508},
{431, 508},
{881, 527},
{225, 527},
{1238, 546},
{844, 546},
{356, 546},
{1050, 565},
{806, 565},
{169, 565},
{131, 565},
{75, 565},
{1181, 583},
{1125, 583},
{1013, 583},
{563, 583},
{413, 583},
{375, 583},
{338, 583},
{19, 583},
{1275, 602},
{788, 602},
{619, 602},
{1331, 621},
{1106, 621},
{975, 621},
{525, 621},
{1238, 640},
{1144, 640},
{1050, 640},
{338, 640},
{75, 640},
{1331, 658},
{1200, 658},
{1088, 658},
{525, 658},
{206, 658},
{1369, 677},
{675, 677},
{581, 677},
{263, 677},
{863, 696},
{488, 696},
{75, 696},
{1425, 715},
{1275, 715},
{919, 715},
{1163, 733},
{994, 733},
{638, 733},
{394, 733},
{356, 733},
{1313, 752},
{1106, 752},
{1069, 752},
{506, 752},
{150, 752},
{281, 771},
{113, 771},
{1425, 790},
{1275, 790},
{1181, 790},
{544, 790},
{244, 790},
{1388, 808},
{1144, 808},
{319, 808},
{1238, 827},
{1106, 827},
{975, 827},
{806, 827},
{525, 827},
{431, 827},
{356, 827},
{94, 827},
{1369, 846},
{1200, 846},
{881, 846},
{656, 846},
{394, 846},
{281, 846},
{169, 846},
{131, 846},
{1481, 865},
{1088, 865},
{356, 865},
{244, 865},
{206, 865},
{1181, 883},
{1125, 883},
{956, 883},
{750, 883},
{450, 883},
{1444, 902},
{1275, 902},
{1088, 902},
{881, 902},
{825, 902},
{788, 902},
{525, 902},
{356, 902},
{1388, 921},
{1031, 921},
{563, 921},
{413, 921},
{1200, 940},
{919, 940},
{863, 940},
{600, 940},
{1106, 958},
{994, 958},
{825, 958},
{750, 958},
{675, 958},
{319, 958},
{281, 958},
{1350, 977},
{788, 977},
{1238, 996},
{1200, 996},
{956, 996},
{694, 996},
{619, 996},
{450, 996},
{394, 996},
{1463, 1015},
{731, 1015},
{1088, 1033},
{638, 1033},
{1313, 1052},
{1013, 1052},
{900, 1052},
{863, 1052},
{825, 1052},
{675, 1052},
{263, 1052},
{1350, 1071},
{1238, 1071},
{1200, 1071},
{1313, 1090},
{788, 1090},
{1275, 1108},
{825, 1108},
{713, 1108},
{656, 1108},
{619, 1108},
{544, 1108},
{1069, 1127},
{1200, 1146},
{300, 1146},
{1275, 1165},
{1106, 1165},
{1050, 1165},
{1350, 1183},
{1313, 1183},
{1238, 1183},
{825, 1183},
{1125, 1202},
{300, 1202},
{1219, 1221},
{1069, 1221},
{1331, 1240},
{1163, 1240},
};*/