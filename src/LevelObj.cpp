#include "LevelObj.h"
#include "LevelPopup.h"

using namespace geode::prelude;

const char* getTextureNameForIconByDifficultyId(int id) { //TODO draw some new textures for it
	switch (id) {
	case 0:
		return "difficulty_auto_btn_001.png";
	case 1:
		return "difficulty_01_btn_001.png";
	case 2:
		return "difficulty_02_btn_001.png";
	case 3:
		return "difficulty_03_btn_001.png";
	case 4:
		return "difficulty_04_btn_001.png";
	case 5:
		return "difficulty_05_btn_001.png";
	case 6:
		return "difficulty_07_btn2_001.png";
	case 7:
		return "difficulty_08_btn2_001.png";
	case 8:
		return "difficulty_06_btn2_001.png";
	case 9:
		return "difficulty_09_btn2_001.png";
	case 10:
		return "difficulty_10_btn2_001.png";
	default:
		return "difficulty_00_btn_001.png";
	}
}

bool LevelObj::init(json::Value lvlJson) {
	if (!CCMenu::init()) return false;
	
	this->lvlJson = lvlJson;
	sprite = CCSprite::createWithSpriteFrameName("level.png"_spr);
	sprite->setScale(0.8);
	button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(LevelObj::onClick));
	button->setAnchorPoint({ 0.5, 0.25 });
	this->setPosition({ float(lvlJson["x"].as_int()), float(lvlJson["y"].as_int()) - sprite->getContentSize().height / 4 });
	this->addChild(button);

	difficultyIcon = CCSprite::createWithSpriteFrameName(getTextureNameForIconByDifficultyId(lvlJson["difficulty"].as_int()));
	difficultyIcon->setScale(0.4);
	difficultyIcon->setPosition({sprite->getContentSize().width / 2 + 1, 28});
	sprite->addChild(difficultyIcon);

	nameLabel = CCLabelBMFont::create(lvlJson["name"].as_string().c_str(), "bigFont.fnt");
	nameLabel->limitLabelWidth(50, 0.45, 0.05);
	nameLabel->setPosition({ sprite->getContentSize().width / 2, sprite->getContentSize().height + 10 });
	sprite->addChild(nameLabel);

	return true;
}

void LevelObj::onClick(CCObject* obj) {
	LevelPopup::create(lvlJson, updateMapAndCoinsCallback)->show();
}


LevelObj* LevelObj::create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback) {
	auto ret = new LevelObj();
	ret->updateMapAndCoinsCallback = updateMapAndCoinsCallback;
	if (ret && ret->init(lvlJson)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}