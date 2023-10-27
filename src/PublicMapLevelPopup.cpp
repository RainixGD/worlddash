#include "./PublicMapLevelPopup.h"
#include "./ReqManager.h"
#include "./DataManager.h"
#include "./PublicLevelAlert.h"
#include "./MyLevelTools.h"
#include "./CounterNode.h"
#include <functional>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;


bool PublicMapLevelPopup::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("Map Level Builder");
	auto size = CCDirector::sharedDirector()->getWinSize();

	m_closeBtn->setZOrder(5);

	auto corner1 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner1->setPosition({ m_size.width / 2 - corner1->getContentSize().width / 2, m_size.height / 2 - corner1->getContentSize().height / 2 });
	corner1->setRotation(180);
	m_buttonMenu->addChild(corner1);

	auto corner2 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner2->setPosition({ -m_size.width / 2 + corner2->getContentSize().width / 2, m_size.height / 2 - corner2->getContentSize().height / 2 });
	corner2->setRotation(90);
	m_buttonMenu->addChild(corner2);

	auto corner3 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner3->setPosition({ m_size.width / 2 - corner3->getContentSize().width / 2, -m_size.height / 2 + corner3->getContentSize().height / 2 });
	corner3->setRotation(270);
	m_buttonMenu->addChild(corner3);

	auto corner4 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner4->setPosition({ -m_size.width / 2 + corner4->getContentSize().width / 2, -m_size.height / 2 + corner4->getContentSize().height / 2 });
	m_buttonMenu->addChild(corner4);


	auto firstBorder = CCScale9Sprite::create("square02_001.png", CCRect(0, 0, 80, 80));
	firstBorder->setContentSize({ 200, 145 });
	firstBorder->setPosition({ -80, 30 });
	firstBorder->setOpacity(50);
	this->m_buttonMenu->addChild(firstBorder);

	auto secondBorder = CCScale9Sprite::create("square02_001.png", CCRect(0, 0, 80, 80));
	secondBorder->setContentSize({ 155, 215 });
	secondBorder->setPosition({ 100, -5 });
	secondBorder->setOpacity(50);
	this->m_buttonMenu->addChild(secondBorder);


	auto cellsArray = CCArray::create();
	auto cellIndexCounter = 0;
	for (json::Value leveljson : DataManager::getInstance()->localLevels) {
		auto cell = GenericListCell::create(std::to_string(cellIndexCounter).c_str(), CCSize(180, 40));

		auto labelMenu = CCMenu::create();
		labelMenu->setPosition(70, 20);
		cell->m_mainLayer->addChild(labelMenu);

		auto levelLabel = CCLabelBMFont::create(leveljson["levelName"].as_string().c_str(), "bigFont.fnt");
		levelLabel->limitLabelWidth(120, 0.7, 0.1);

		auto btn = CCMenuItemSpriteExtra::create(levelLabel, this, menu_selector(PublicMapLevelPopup::onLevelInListClicked));
		btn->setTag(leveljson["id"].as_int());
		labelMenu->addChild(btn);

		cellsArray->addObject(cell);
		cellIndexCounter++;
	}

	auto list = ListView::create(cellsArray, 40, 140, 150);
	list->setPosition({ (list->getContentSize().width) / 2 - 40, (-list->getContentSize().height) / 2 + 5 });
	this->m_buttonMenu->addChild(list);


	auto selectLevelOpstions = CCLabelBMFont::create("Select your level:", "bigFont.fnt");
	selectLevelOpstions->setPosition({ 100, 95 });
	selectLevelOpstions->setScale(0.4);
	this->m_buttonMenu->addChild(selectLevelOpstions);

	auto headerLabel = CCLabelBMFont::create("Rent Options:", "bigFont.fnt");
	headerLabel->setPosition({ -80, 95 });
	headerLabel->setScale(0.4);
	this->m_buttonMenu->addChild(headerLabel);

	auto selectedLevelInfo = CCLabelBMFont::create("Selected Level:", "goldFont.fnt");
	selectedLevelInfo->setPosition({ 100, -80 });
	selectedLevelInfo->setScale(0.6);
	this->m_buttonMenu->addChild(selectedLevelInfo);

	selectedLevel = CCLabelBMFont::create("", "goldFont.fnt");
	selectedLevel->setPosition({ 100, -100 });
	selectedLevel->limitLabelWidth(150, 0.8, 0.1);
	this->m_buttonMenu->addChild(selectedLevel);

	auto counterInfoLabel = CCLabelBMFont::create("The level will be on the map:", "goldFont.fnt");
	counterInfoLabel->setScale(0.5);
	counterInfoLabel->setPosition({ -80, 75 });
	this->m_buttonMenu->addChild(counterInfoLabel);

	auto counterNode = CounterNode::create(1, 1, 1, 200, [this](int value) {
		onTimeCounterNodeChanged(value);
	});

	counterNode->setPosition({ -115, 35 });
	this->m_buttonMenu->addChild(counterNode);

	auto daysLabel = CCLabelBMFont::create("days", "goldFont.fnt");
	daysLabel->setPosition({ -20, 35 });
	this->m_buttonMenu->addChild(daysLabel);

	auto resultLabelInfo = CCLabelBMFont::create("You should pay for it:", "goldFont.fnt");
	resultLabelInfo->setPosition({ -80, 0 });
	resultLabelInfo->setScale(0.6);
	this->m_buttonMenu->addChild(resultLabelInfo);

	resultLabel = CCLabelBMFont::create(std::to_string(coinsProDays).c_str(), "goldFont.fnt");
	resultLabel->setPosition({ -80, -20 });
	this->m_buttonMenu->addChild(resultLabel);

	coin = CCSprite::createWithSpriteFrameName("susCoin.png"_spr);
	this->m_buttonMenu->addChild(coin);
	fixCoinPosition();

	auto publicBtnSprite = ButtonSprite::create("public");
	publicBtn = CCMenuItemSpriteExtra::create(publicBtnSprite, this, menu_selector(PublicMapLevelPopup::publicLevel));
	publicBtn->setPosition({ -80, -90 });
	publicBtn->setVisible(0);
	this->m_buttonMenu->addChild(publicBtn);

	return true;
}

void PublicMapLevelPopup::fixCoinPosition() {
	coin->setPosition({ resultLabel->getPosition().x + resultLabel->getContentSize().width / 2 + coin->getContentSize().width / 2, resultLabel->getPosition().y });
}

void PublicMapLevelPopup::onTimeCounterNodeChanged(int value) {
	days = value;
	coins = days * coinsProDays;
	resultLabel->setString(std::to_string(coins).c_str());
	fixCoinPosition();
}

void PublicMapLevelPopup::publicLevel(CCObject*) {
	createQuickPopup(
		"confirm alert",
		"Do you really want to public this level?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				PublicLevelAlert::create(this)->show();
			}
		}
	);
}

void PublicMapLevelPopup::onLevelInListClicked(CCObject* obj) {
	auto btn = static_cast<CCNode*>(obj);
	selectedLevelId = btn->getTag();
	selectedLevel->setString((DataManager::getInstance()->getLocalLevelById(selectedLevelId))["levelName"].as_string().c_str());
	publicBtn->setVisible(1);
}

void PublicMapLevelPopup::onClose(CCObject* obj) {
	this->setKeyboardEnabled(false);
	this->removeFromParentAndCleanup(true);
}


PublicMapLevelPopup* PublicMapLevelPopup::create(int x, int y, std::function<void()> updateMapAndCoinsCallback) {
	auto ret = new PublicMapLevelPopup();
	ret->selectedPlusX = x;
	ret->selectedPlusY = y;
	ret->updateMapAndCoinsCallback = updateMapAndCoinsCallback;
	if (ret && ret->init(400.f, 270.f)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}