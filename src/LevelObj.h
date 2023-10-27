#pragma once

using namespace geode::prelude;

class LevelObj : public CCMenu {
	virtual bool init(json::Value lvlJson);
public:
	void onClick(CCObject* obj);

public:
	static LevelObj* create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback);
private:
	std::function<void()> updateMapAndCoinsCallback;
	CCMenuItemSpriteExtra* button;
	CCSprite* sprite;
	CCSprite* difficultyIcon;
	CCLabelBMFont* nameLabel;

	json::Value lvlJson;
};