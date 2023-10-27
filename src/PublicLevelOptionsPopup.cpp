#include "./PublicLevelOptionsPopup.h"
#include "./ReqManager.h"
#include "./DataManager.h"
#include "./PublicLevelAlert.h"
#include "./MyLevelTools.h"
#include "./CounterNode.h"
#include "./UpdateLevelAlert.h"
#include "./PayMoreForPublicLevelAlert.h"
#include <functional>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;


bool PublicLevelOptionsPopup::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("Options");
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

	
	auto firstBorder = CCScale9Sprite::create("square02_small.png", CCRect(0, 0, 40, 40));
	firstBorder->setContentSize({ 200, 51 });
	firstBorder->setPosition({ -80, 77 });
	firstBorder->setOpacity(50);
	this->m_buttonMenu->addChild(firstBorder);

	auto secondBorder = CCScale9Sprite::create("square02_small.png", CCRect(0, 0, 40, 40));
	secondBorder->setContentSize({ 155, 215 });
	secondBorder->setPosition({ 100, -5 });
	secondBorder->setOpacity(50);
	this->m_buttonMenu->addChild(secondBorder);

	auto thirdBorder = CCScale9Sprite::create("square02_small.png", CCRect(0, 0, 40, 40));
	thirdBorder->setContentSize({ 200, 157 });
	thirdBorder->setPosition({ -80, -34 });
	thirdBorder->setOpacity(50);
	this->m_buttonMenu->addChild(thirdBorder);
	

	auto cellsArray = CCArray::create();
	auto cellIndexCounter = 0;
	for (json::Value leveljson : DataManager::getInstance()->localLevels) {
		auto cell = GenericListCell::create(std::to_string(cellIndexCounter).c_str(), CCSize(180, 40));

		auto labelMenu = CCMenu::create();
		labelMenu->setPosition(70, 20);
		cell->m_mainLayer->addChild(labelMenu);

		auto levelLabel = CCLabelBMFont::create(leveljson["levelName"].as_string().c_str(), "bigFont.fnt");
		levelLabel->limitLabelWidth(120, 0.7, 0.1);

		auto btn = CCMenuItemSpriteExtra::create(levelLabel, this, menu_selector(PublicLevelOptionsPopup::onLevelInListClicked));
		btn->setTag(leveljson["id"].as_int());
		labelMenu->addChild(btn);

		cellsArray->addObject(cell);
		cellIndexCounter++;
	}

	auto list = ListView::create(cellsArray, 40, 140, 120);
	list->setPosition({ (list->getContentSize().width) / 2 - 40, (-list->getContentSize().height) / 2 + 25 });
	this->m_buttonMenu->addChild(list);
	

	auto selectLevelOpstions = CCLabelBMFont::create("Update level:", "bigFont.fnt");
	selectLevelOpstions->setPosition({ 100, 95 });
	selectLevelOpstions->setScale(0.4);
	this->m_buttonMenu->addChild(selectLevelOpstions);

	auto selectedLevelInfo = CCLabelBMFont::create("Selected Level:", "goldFont.fnt");
	selectedLevelInfo->setPosition({ 100, -45 });
	selectedLevelInfo->setScale(0.6);
	this->m_buttonMenu->addChild(selectedLevelInfo);

	selectedLevel = CCLabelBMFont::create("", "goldFont.fnt");
	selectedLevel->setPosition({ 100, -65 });
	selectedLevel->limitLabelWidth(150, 0.8, 0.1);
	this->m_buttonMenu->addChild(selectedLevel);

	auto updateLevelBtnSprite = ButtonSprite::create("update level");
	updateLevelBtnSprite->setScale(0.7);
	updateLevelBtn = CCMenuItemSpriteExtra::create(updateLevelBtnSprite, this, menu_selector(PublicLevelOptionsPopup::onUpdateLevel));
	updateLevelBtn->setPosition({ 100, -95 });
	updateLevelBtn->setVisible(0);
	this->m_buttonMenu->addChild(updateLevelBtn);



	auto headerLabel = CCLabelBMFont::create("Rent Info:", "bigFont.fnt");
	headerLabel->setPosition({ -80, 95 });
	headerLabel->setScale(0.4);
	this->m_buttonMenu->addChild(headerLabel);

	auto timeInfoLabel = CCLabelBMFont::create("Your level will be on the map another:", "goldFont.fnt");
	timeInfoLabel->setPosition({ -80, 80 });
	timeInfoLabel->setScale(0.375);
	this->m_buttonMenu->addChild(timeInfoLabel);

	int willBeOnMapTimeUnix = lvlJson["publicTime"].as_int() + lvlJson["rentPeriod"].as_int() - lvlJson["timeNow"].as_int();

	auto timeLabel = CCLabelBMFont::create(unix2DateString(willBeOnMapTimeUnix).c_str(), "goldFont.fnt");
	timeLabel->setScale(0.375);
	timeLabel->setPosition({ -80, 65 });
	this->m_buttonMenu->addChild(timeLabel);


	auto BuyMoreHeaderLabel = CCLabelBMFont::create("Pay more for rent:", "bigFont.fnt");
	BuyMoreHeaderLabel->setScale(0.4);
	BuyMoreHeaderLabel->setPosition({ -80, 35 });
	this->m_buttonMenu->addChild(BuyMoreHeaderLabel);

	auto counterInfoLabel = CCLabelBMFont::create("Pay extra to save your level on the map:", "goldFont.fnt");
	counterInfoLabel->setScale(0.35);
	counterInfoLabel->setPosition({ -80, 15 });
	this->m_buttonMenu->addChild(counterInfoLabel);

	auto counterNode = CounterNode::create(1, 1, 1, 200, [this](int value) {
		onTimeCounterNodeChanged(value);
	});
	counterNode->setPosition({ -115, -15 });
	this->m_buttonMenu->addChild(counterNode);

	auto daysLabel = CCLabelBMFont::create("days", "goldFont.fnt");
	daysLabel->setPosition({ -20, -15 });
	this->m_buttonMenu->addChild(daysLabel);

	auto resultLabelInfo = CCLabelBMFont::create("You should pay for it:", "goldFont.fnt");
	resultLabelInfo->setPosition({ -80, -40 });
	resultLabelInfo->setScale(0.6);
	this->m_buttonMenu->addChild(resultLabelInfo);

	resultLabel = CCLabelBMFont::create(std::to_string(coinsProDays).c_str(), "goldFont.fnt");
	resultLabel->setPosition({ -80, -65 });
	this->m_buttonMenu->addChild(resultLabel);

	coin = CCSprite::createWithSpriteFrameName("susCoin.png"_spr);
	this->m_buttonMenu->addChild(coin);
	fixCoinPosition();

	auto payMoreForLevelBtnSprite = ButtonSprite::create("pay");
	payMoreForLevelBtnSprite->setScale(0.7);
	payMoreForLevelBtn = CCMenuItemSpriteExtra::create(payMoreForLevelBtnSprite, this, menu_selector(PublicLevelOptionsPopup::onPayMoreForLevel));
	payMoreForLevelBtn->setPosition({ -80, -95 });
	this->m_buttonMenu->addChild(payMoreForLevelBtn);

	return true;
}

std::string PublicLevelOptionsPopup::unix2DateString(int seconds) {
	std::string endString = "";
	if (seconds >= 60 * 60 * 24) {
		auto days = seconds / (60 * 60 * 24);
		seconds -= days * (60 * 60 * 24);
		endString += (std::to_string(days) + " days ");
	}
	if (seconds >= 60 * 60) {
		auto hours = seconds / (60 * 60);
		seconds -= hours * (60 * 60);
		endString += (std::to_string(hours) + " hours ");
	}
	if (seconds >= 60) {
		auto minutes = seconds / (60);
		seconds -= minutes * (60);
		endString += (std::to_string(minutes) + " minutes ");
	}
	endString += (std::to_string(seconds) + " seconds ");
	return endString;
}

void PublicLevelOptionsPopup::fixCoinPosition() {
	coin->setPosition({ resultLabel->getPosition().x + resultLabel->getContentSize().width / 2 + coin->getContentSize().width / 2, resultLabel->getPosition().y });
}

void PublicLevelOptionsPopup::onTimeCounterNodeChanged(int value) {
	days = value;
	coins = days * coinsProDays;
	resultLabel->setString(std::to_string(coins).c_str());
	fixCoinPosition();
}

void PublicLevelOptionsPopup::onUpdateLevel(CCObject*) {
	createQuickPopup(
		"confirm alert",
		"Do you really want to <cg>update</c> this level?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				UpdateLevelAlert::create(this)->show();
			}
		}
	);
}

void PublicLevelOptionsPopup::onPayMoreForLevel(CCObject*) {
	createQuickPopup(
		"confirm alert",
		"Do you really want to pay more for this level?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				PayMoreForPublicLevelAlert::create(this)->show();
			}
		}
	);
}


void PublicLevelOptionsPopup::onLevelInListClicked(CCObject* obj) {
	auto btn = static_cast<CCNode*>(obj);
	selectedLevelId = btn->getTag();
	selectedLevel->setString((DataManager::getInstance()->getLocalLevelById(selectedLevelId))["levelName"].as_string().c_str());
	updateLevelBtn->setVisible(1);
}

void PublicLevelOptionsPopup::onClose(CCObject* obj) {
	this->setKeyboardEnabled(false);
	this->removeFromParentAndCleanup(true);
}


PublicLevelOptionsPopup* PublicLevelOptionsPopup::create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback, std::function<void()> closeLevelPopupCallback) {
	auto ret = new PublicLevelOptionsPopup();
	ret->lvlJson = lvlJson;
	ret->updateMapAndCoinsCallback = updateMapAndCoinsCallback;
	ret->closeLevelPopupCallback = closeLevelPopupCallback;
	if (ret && ret->init(400.f, 270.f)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}