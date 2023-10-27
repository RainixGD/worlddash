#pragma once
#include "LevelObj.h"
#include "PublicMapLevelPopup.h"
#include "MapDecoContainer.h"

using namespace geode::prelude;

class MapUiLayer;

class MapLayer : public CCLayer {
private:
	const int levelTag = 69;
	const int plusTag = 96;
	json::Array mapLevels;
protected:
	virtual bool init();
public:
	bool isAddLevelMode = 0;
	MapUiLayer* mapUiLayer;
	CCSprite* map;
	MapDecoContainer* mapDecoContainer;

	CCMenu* levelsMenu;

	MapLayer();
	static MapLayer* create(MapUiLayer* mapUiLayer);
	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	void updateMap(json::Array newLevels);
	void makeUpdateMapReq();
	void updateMapAndCoins();

	bool isExistLevelByCordsInJson(int x, int y) {
		for (json::Value mapLevel : mapLevels) {
			if (mapLevel["x"].as_int() == x && mapLevel["y"].as_int() == y)
				return true;
		}
		return false;
	}

	void hideLevels() {
		CCObject* obj;
		CCARRAY_FOREACH(levelsMenu->getChildren(), obj) {
			auto node = static_cast<CCNode*>(obj);
			if (node->getTag() == levelTag)
				node->setVisible(0);
		}
	}
	void showLevels() {
		CCObject* obj;
		CCARRAY_FOREACH(levelsMenu->getChildren(), obj) {
			auto node = static_cast<CCNode*>(obj);
			if (node->getTag() == levelTag)
				node->setVisible(1);
		}
	}
	void hideAddBtns() {
		CCObject* obj;
		CCARRAY_FOREACH(levelsMenu->getChildren(), obj) {
			auto node = static_cast<CCNode*>(obj);
			if (node->getTag() == plusTag)
				node->setVisible(0);
		}
	}
	void showAddBtns() {
		CCObject* obj;
		CCARRAY_FOREACH(levelsMenu->getChildren(), obj) {
			auto node = static_cast<CCNode*>(obj);
			if (node->getTag() == plusTag)
				node->setVisible(1);
		}
	}
	
	void clearMap() {
		levelsMenu->removeFromParentAndCleanup(true);
		levelsMenu = CCMenu::create();
		levelsMenu->setPosition(0, 0);
		map->addChild(levelsMenu);
	}

	void onPlusBtnClicked(CCObject* obj) {
		auto plusBtnMenu = static_cast<CCNode*>(obj)->getParent();
		PublicMapLevelPopup::create(int(plusBtnMenu->getPosition().x), int(plusBtnMenu->getPosition().y), [this]() {this->updateMapAndCoins(); })->show();
	}
private:
	bool isDragging;
	cocos2d::CCPoint lastTouchPos;
	CCPoint minScroll = { 0, 0 };
	CCPoint maxScroll = { 0, 0 };

	static int availableMapPoints[235][2];
};