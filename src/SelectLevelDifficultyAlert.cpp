#include "./SelectLevelDifficultyAlert.h"
#include "./Button.h"
#include "./RateDifficultyLevelAlert.h"
using namespace geode::prelude;

const char* getTextureNameByDifficultyIdForRateAlert(int id) {
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

bool SelectLevelDifficultyAlert::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("Select Difficulty");

	float firstRowCounterX = -175;
	float secondRowCounterX = -150;

	interfaceButtons = CCMenu::create();
	interfaceButtons->setPosition({ 0, 0 });
	m_buttonMenu->addChild(interfaceButtons);

	for (int i = 0; i <= 10; i++) {
		auto difficultyBtnSprite = CCSprite::createWithSpriteFrameName(getTextureNameByDifficultyIdForRateAlert(i));
		auto difficultyBtn = CCMenuItemSpriteExtra::create(difficultyBtnSprite, this, menu_selector(SelectLevelDifficultyAlert::onSelect));
		difficultyBtn->setTag(i);
		difficultyBtn->setColor(ccc3(100, 100, 100));
		i < 6 ? difficultyBtn->setPosition({ firstRowCounterX += 50, 40 }) : difficultyBtn->setPosition({ secondRowCounterX += 50, -20 });
		interfaceButtons->addChild(difficultyBtn);
	}

	auto rateButtonSprite = ButtonSprite::create("rate");
	auto rateButton = CCMenuItemSpriteExtra::create(rateButtonSprite, this, menu_selector(SelectLevelDifficultyAlert::onRate));
	rateButton->setPositionY(-80);
	m_buttonMenu->addChild(rateButton);

	updateInterface();

	return true;
}

void SelectLevelDifficultyAlert::onSelect(CCObject* button) {
	auto tag = button->getTag();
	selectedDifficulty = tag;
	log::info("{}",selectedDifficulty);
	updateInterface();
}

void SelectLevelDifficultyAlert::onRate(CCObject*) {
	RateDifficultyLevelAlert::create(levelId, selectedDifficulty)->show();
	this->keyBackClicked();
}

void SelectLevelDifficultyAlert::updateInterface() {
	CCObject* obj;
	CCARRAY_FOREACH(interfaceButtons->getChildren(), obj) {
		auto btn = static_cast<CCMenuItemSpriteExtra*>(obj);
		if (btn->getTag() == selectedDifficulty)
			btn->setColor(ccc3(255, 255, 255));
		else
			btn->setColor(ccc3(100, 100, 100));
	}
}

SelectLevelDifficultyAlert* SelectLevelDifficultyAlert::create(int levelId) {
	auto ret = new SelectLevelDifficultyAlert();
	ret->levelId = levelId;
	if (ret && ret->init(350.f, 230.f)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}